//
// Created by Алексей Сиротин on 19.01.2022.
//

#include <QDebug>
#include <QThread>
#include "mono.h"

class QTest : public QObject {

};

int main(int argc, char *argv[]) {
	Mono<QObject>::create([](MonoSink<QObject> *monoSink) {
	  QObject *i = new QObject();
	  qDebug() << "OBJECT" << i;
	  monoSink->success(i);
	  qDebug() << QThread::currentThreadId();
	})
		->subscribeOn()
		->map<QTest>([](QObject *value) {
			return new QTest();
		})
		->subscribe([](QTest *value) {
		  qDebug() << QThread::currentThreadId();
		  qDebug() << value;
		}, [](const std::exception& exception) {
		  qDebug() << exception.what();
		});

	qDebug() << "MAIN"  << QThread::currentThreadId();
	QThread::sleep(5);
}
