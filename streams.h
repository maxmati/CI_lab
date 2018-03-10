#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <experimental/optional>

namespace std { using experimental::optional; } // NOLINT

namespace stream {
    template<typename T>
    class StreamSource {
    public:
        virtual std::optional<T> pop() = 0;
    };

    template<typename OutT, typename InT>
    class MapStreamSource : public StreamSource<OutT> {
    public:
        MapStreamSource(std::unique_ptr<StreamSource<InT>> &&source,
                        const std::function<OutT(InT)> &mapper)
                : source(std::move(source)),
                  mapper(mapper) {}

        std::optional<OutT> pop() override {
            std::optional<InT> elem = source->pop();

            if (elem) {
                return mapper(std::move(*elem));
            } else {
                return {};
            }
        }

    private:
        std::unique_ptr<StreamSource<InT>> source;
        std::function<OutT(InT)> mapper;
    };

    template<typename In1T, typename In2T>
    class ZipStreamSource : public StreamSource<std::pair<In1T, In2T>> {
    public:
        ZipStreamSource(std::unique_ptr<StreamSource<In1T>> &&source1,
                        std::unique_ptr<StreamSource<In2T>> &&source2)
                : source1(std::move(source1)), source2(std::move(source2)) {}

        std::optional<std::pair<In1T, In2T>> pop() override {
            std::optional<In1T> elem1 = source1->pop();
            std::optional<In2T> elem2 = source2->pop();

            if (elem1 && elem2) {
                return std::make_pair(*elem1, *elem2);
            } else {
                return {};
            }
        }

    private:
        std::unique_ptr<StreamSource<In1T>> source1;
        std::unique_ptr<StreamSource<In2T>> source2;
    };

    template<typename T>
    class FilterStreamSource : public StreamSource<T> {
    public:
        FilterStreamSource(std::unique_ptr<StreamSource<T>> &&source,
                           const std::function<bool(const T &)> &pred)
                : source(std::move(source)),
                  pred(pred) {}

        std::optional<T> pop() override {
            std::optional<T> elem = source->pop();
            while (elem && !pred(*elem)) {
                elem = source->pop();
            }

            return elem;
        }

    private:
        std::unique_ptr<StreamSource<T>> source;
        std::function<bool(const T &)> pred;
    };

    template<typename T>
    class LimitStreamSource : public StreamSource<T> {
    public:
        LimitStreamSource(std::unique_ptr<StreamSource<T>> &&source,
                          const size_t limit)
                : source(std::move(source)),
                  limit(limit) {}

        std::optional<T> pop() override {
            if (count >= limit)
                return {};

            ++count;
            return source->pop();
        }

    private:
        std::unique_ptr<StreamSource<T>> source;
        const size_t limit;
        size_t count = 0;
    };

    template<typename IteratorT>
    class IteratorStreamSource : public StreamSource<typename IteratorT::value_type> {
    public:
        IteratorStreamSource(IteratorT first, IteratorT last) : first(first), last(last) {}

        std::optional<typename IteratorT::value_type> pop() override {
            if (first == last) return {};
            return std::move(*first++);
        }

    private:
        IteratorT first;
        IteratorT last;
    };

    template<typename T>
    class PushBackColector {
    public:
        typedef T result_type;

        result_type collect(StreamSource<typename T::value_type> *source) {
            T vector;
            std::optional<typename T::value_type> elem = source->pop();
            while (elem) {
                vector.push_back(std::move(*elem));
                elem = source->pop();
            }

            return std::move(vector);
        }
    };

    template<typename T>
    class Stream {
    public:
        explicit Stream(std::unique_ptr<StreamSource<T>> &&source)
                : source(std::move(source)) {}

        template<typename OutT>
        Stream<OutT> map(std::function<OutT(T)> mapper) {
            auto s = std::unique_ptr<StreamSource<OutT>>(new MapStreamSource<OutT, T>(std::move(source), mapper));

            return Stream<OutT>(std::move(s));
        }

        Stream<T> forEach(std::function<void(T)> consumer) {
            map<T>([&](auto item){ consumer(item); return item; });
        }

        template<typename InT>
        Stream<std::pair<T, InT>> zip(Stream<InT> str) {
            auto s = std::unique_ptr<StreamSource<std::pair<T, InT>>>(
                    new ZipStreamSource<T, InT>(std::move(source), std::move(str.source)));

            return Stream<std::pair<T, InT>>(std::move(s));
        }

        void flush(){
            while(source->pop());
        }

        template<typename CollectorT>
        typename CollectorT::result_type collect(CollectorT collector) {
            return collector.collect(source.get());
        }

        Stream<T> filter(std::function<bool(const T &)> pred) {
            auto s = std::unique_ptr<StreamSource<T>>(new FilterStreamSource<T>(std::move(source), pred));

            return Stream(std::move(s));
        }

        template<typename AgregatedT>
        AgregatedT reduce(std::function<AgregatedT(T, AgregatedT)> func, AgregatedT neutral) {
            std::optional<T> elem = source->pop();
            while (elem) {
                neutral = func(*elem, neutral);
                elem = source->pop();
            }
        }

        std::optional<T> first() {
            return source->pop();
        }

        Stream limit(size_t size) {
            auto s = std::unique_ptr<StreamSource<T>>(new LimitStreamSource<T>(std::move(source), size));

            return Stream(std::move(s));
        }

    public: //TODO
        std::unique_ptr<StreamSource<T>> source;
    };

    template<typename IteratorT>
    Stream<typename IteratorT::value_type> fromRange(IteratorT first, IteratorT last) {
        auto s = std::make_unique<IteratorStreamSource<IteratorT>>(first, last);
        return Stream<typename IteratorT::value_type>(std::move(s));
    }

    template<typename T>
    auto fromEnumerable(T &enumerable) {
        return fromRange(std::begin(enumerable), std::end(enumerable));
    }
}