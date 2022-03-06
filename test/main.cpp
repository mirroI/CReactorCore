//
// Created by Алексей Сиротин on 19.01.2022.
//

#include <QDebug>
#include <QThread>
#include "mono.h"

int main(int argc, char *argv[]) {
	Mono<int>::create([](MonoSink<int> *monoSink) {
	  int *i = new int(1);
	  monoSink->success(i);
	  qDebug() << QThread::currentThreadId();
	})
		->subscribeOn()
		->map<int>([](int *value) {
			return new int(2);
		})
		->subscribe([](int *value) {
		  qDebug() << QThread::currentThreadId();
		  qDebug() << *value;
		}, [](const std::exception& exception) {
		  qDebug() << exception.what();
		});

	qDebug() << "MAIN"  << QThread::currentThreadId();
	QThread::sleep(5);


}
