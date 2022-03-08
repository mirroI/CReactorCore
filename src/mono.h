//
// Created by Алексей Сиротин on 19.01.2022.
//

#ifndef REACTORCORE_SRC_MONO_H
#define REACTORCORE_SRC_MONO_H

#include <QRunnable>
#include "operators.h"
#include "publisher.h"
#include "mono_sink.h"

#include "lambda_mono_subscriber.h"
#include "optimizable_operator.h"

template<typename T>
class Mono : public CorePublisher<T> {
 protected:
	explicit Mono(QObject *parent);

 public:
	static Mono<T> *create(std::function<void(MonoSink<T> *monoSink)> callback, QObject *parent = nullptr);

	Mono<T> *subscribeOn();

	template<typename OUT>
	Mono<OUT> *map(std::function<OUT *(T *)> mapper);

	void subscribe(std::function<void(T *)> consumer,
		std::function<void(const std::exception& exception)> errorConsumer = nullptr);

	void subscribe(Subscriber<T> *subscriber) override;
	virtual void subscribe(CoreSubscriber<T> *subscriber) override = 0;
};

#include "mono_create.h"
#include "mono_subscribe_on.h"
#include "mono_map.h"

template<typename T>
Mono<T>::Mono(QObject *parent) : CorePublisher<T>(parent) {
}

template<typename T>
Mono<T> *Mono<T>::create(std::function<void(MonoSink<T> *monoSink)> callback, QObject *parent) {
	return new MonoCreate<T>(callback, parent);
}

template<typename T>
Mono<T> *Mono<T>::subscribeOn() {
	return new MonoSubscribeOn<T>((Mono<T> *) this, nullptr);
}

template<typename T>
template<typename OUT>
Mono<OUT> *Mono<T>::map(std::function<OUT *(T *)> mapper) {
	return new MonoMap<T, OUT>(this, mapper, this);
}

template<typename T>
void Mono<T>::subscribe(std::function<void(T *)> consumer, std::function<void(const std::exception&)> errorConsumer) {
	subscribe(static_cast<Subscriber<T> *>(new LambdaMonoSubscriber<T>{ consumer, errorConsumer }));
}

template<typename T>
void Mono<T>::subscribe(Subscriber<T> *actual) {
	CorePublisher<T> *publisher = this;
	CoreSubscriber<T> *subscriber = Operators::toCoreSubscriber<T>(actual);

	//Нужно достать родителя, которого создал метод create так как я не уверен что сейчас в нем
	//Сделать вызов core subscriber

	publisher->subscribe(subscriber);
}

#endif //REACTORCORE_SRC_MONO_H