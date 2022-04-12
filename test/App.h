//
// Created by Алексей Сиротин on 09.04.2022.
//

#ifndef REACTORCORE_TEST_APP_H
#define REACTORCORE_TEST_APP_H

#include <QDebug>
#include <QThread>

#include "mono.h"

class App : public QThread {
 Q_OBJECT

 public:
	void run() {
		qDebug() << *Mono<int>::create([](MonoSink<int> *monoSink) {
			int i = 0;
		  while (i <= 5) {
			  i++;
			  QThread::sleep(1);
			  qDebug() << "TEST" << i;
		  }

		  monoSink->success(new int(1));
		})
			->subscribeOn()
			->map<double>([](int *value) {
			  return new double(2.2);
			})
			->block();
//			->subscribe([=](double *value) {
//			  qDebug() << *value;
//			}, [](const std::exception& exception) {
//			  qDebug() << exception.what();
//			});

qDebug() << "PIDOR";
	}
};

#endif //REACTORCORE_TEST_APP_H
