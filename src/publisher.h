//
// Created by Алексей Сиротин on 19.01.2022.
//

#ifndef REACTORCORE_SRC_PUBLISHER_H
#define REACTORCORE_SRC_PUBLISHER_H

#include <functional>

#include "subscriber.h"

template<typename T>
class Publisher : public QObject {
 public:
	explicit Publisher(QObject *parent);

	virtual void subscribe(Subscriber<T> *subscriber) = 0;
};

template<typename T>
class CorePublisher : public Publisher<T> {
 public:
	explicit CorePublisher(QObject *parent);

	virtual void subscribe(CoreSubscriber<T> *subscriber) = 0;
};

template<typename T>
Publisher<T>::Publisher(QObject *parent):QObject(parent) {
}

template<typename T>
CorePublisher<T>::CorePublisher(QObject *parent):Publisher<T>(parent) {

}

#endif //REACTORCORE_SRC_PUBLISHER_H
