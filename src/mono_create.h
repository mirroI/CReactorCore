//
// Created by Алексей Сиротин on 20.02.2022.
//

#ifndef REACTORCORE_SRC_MONO_CREATE_H
#define REACTORCORE_SRC_MONO_CREATE_H

#include "mono.h"

template<typename T>
class MonoCreate : public Mono<T> {
 private:
	const std::function<void(MonoSink<T> *monoSink)> _callback;

 public:
	explicit MonoCreate(std::function<void(MonoSink<T> *monoSink)> callback, QObject *parent);

	void subscribe(CoreSubscriber<T> *subscriber) override;
};

template<typename T>
MonoCreate<T>::MonoCreate(std::function<void(MonoSink<T> *)> callback, QObject *parent)
	: _callback(callback), Mono<T>(parent) {

}

template<typename T>
void MonoCreate<T>::subscribe(CoreSubscriber<T> *actual) {
	DefaultMonoSink<T> *emitter = new DefaultMonoSink<T>(actual, this);

	actual->onSubscribe(emitter);

	try {
		_callback(emitter);
	}
	catch (std::exception e) {
		emitter->error(e);
	}
}

#endif //REACTORCORE_SRC_MONO_CREATE_H
