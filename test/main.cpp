//
// Created by Алексей Сиротин on 19.01.2022.
//


#include <QCoreApplication>

#include "App.h"

int main(int argc, char *argv[]) {
	QCoreApplication *a = new QCoreApplication(argc, argv);

	App *app = new App();
	app->start();

	return a->exec();
}
