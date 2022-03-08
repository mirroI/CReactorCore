//
// Created by Алексей Сиротин on 20.02.2022.
//

#ifndef CREACTORCORE_SRC_MONO_OPERATOR_H
#define CREACTORCORE_SRC_MONO_OPERATOR_H

#include "mono.h"

template<typename IN, typename OUT>
class MonoOperator : public Mono<OUT> {
 protected:
	Mono<IN> *_source;

 protected:
	MonoOperator(Mono<IN> *source, QObject *parent);
};

template<typename IN, typename OUT>
class InternalMonoOperator : public MonoOperator<IN, OUT>, public OptimizableOperator<OUT, IN> {
 private:
	OptimizableOperator<QObject, IN> *_optimizableOperator = nullptr;

 protected:
	InternalMonoOperator(Mono<IN> *source, QObject *parent);

 public:
	virtual CoreSubscriber<IN> *subscribeOrReturn(CoreSubscriber<OUT> *actual) override = 0;
	CorePublisher<IN> *source() override;
	OptimizableOperator<QObject, IN> *nextOptimizableSource() override;

 public:
	void subscribe(Subscriber<OUT> *subscriber) override;
	void subscribe(CoreSubscriber<OUT> *subscriber) override;
};

template<typename IN, typename OUT>
MonoOperator<IN, OUT>::MonoOperator(Mono<IN> *source, QObject *parent) : _source(source), Mono<OUT>(parent) {
}

template<typename IN, typename OUT>
InternalMonoOperator<IN, OUT>::InternalMonoOperator(Mono<IN> *source, QObject *parent):MonoOperator<IN, OUT>(source,
	parent), OptimizableOperator<OUT, IN>(parent) {
	if (OptimizableOperator<QObject, IN>
		*optimizableOperator = dynamic_cast<OptimizableOperator<QObject, IN> *>(source)) {
		_optimizableOperator = optimizableOperator;
	}
}

template<typename IN, typename OUT>
CorePublisher<IN> *InternalMonoOperator<IN, OUT>::source() {
	return MonoOperator<IN, OUT>::_source;
}

template<typename IN, typename OUT>
OptimizableOperator<QObject, IN> *InternalMonoOperator<IN, OUT>::nextOptimizableSource() {
	return _optimizableOperator;
}

template<typename IN, typename OUT>
void InternalMonoOperator<IN, OUT>::subscribe(Subscriber<OUT> *subscriber) {
	Mono<OUT>::subscribe(subscriber);
}

template<typename IN, typename OUT>
void InternalMonoOperator<IN, OUT>::subscribe(CoreSubscriber<OUT> *subscriber) {
	OptimizableOperator<QObject, OUT> *optimizableOperator = (OptimizableOperator<QObject, OUT> *) this;

	while (true) {
		subscriber = optimizableOperator->subscribeOrReturn((CoreSubscriber<QObject> *) subscriber);
		if (subscriber == nullptr) {
			return;
		}

		OptimizableOperator<QObject, OUT> *newOptimizableOperator = optimizableOperator->nextOptimizableSource();
		if (newOptimizableOperator == nullptr) {
			return optimizableOperator->source()->subscribe(subscriber);
		}

		optimizableOperator = newOptimizableOperator;
	}
}

#endif //CREACTORCORE_SRC_MONO_OPERATOR_H
