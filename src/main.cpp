/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <locale.h>

#include <QtWidgets>

#include <DLog>
#include <DMainWindow>
#include <DApplication>
#include <dutility.h>

#include "config.h"

#include "mainwindow.h"

DWIDGET_USE_NAMESPACE
DUTIL_USE_NAMESPACE


int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

#if defined(STATIC_LIB)
    DWIDGET_INIT_RESOURCE();
#endif

    DApplication app(argc, argv);

    // required by mpv
    setlocale(LC_NUMERIC, "C");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("file", QCoreApplication::tr("Movie file path"));
    parser.process(app);

    QString toOpenFile;
    if (1 == parser.positionalArguments().length()) {
        toOpenFile = parser.positionalArguments().first();
    }

    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-movie");
    app.setApplicationVersion(DMR_VERSION);
    app.setProductIcon(QPixmap(":/resources/icons/logo-big.svg"));
    app.setProductName(QT_TR_NOOP("Deepin Movie"));
    app.setApplicationLicense(QT_TR_NOOP("GPL v3"));
    app.setApplicationDescription(QT_TR_NOOP(
                "Deepin Movie is a well-designed and full-featured"
                " video player with simple borderless design. It supports local and"
                " streaming media play with multiple video formats."));

    app.setTheme("dark");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    qDebug() << "log path: " << DLogManager::getlogFilePath();

    if (!app.setSingleInstance("deepinmovie")) {
        qDebug() << "another deepin movie instance has started";
        exit(0);
    }

    app.loadTranslator();

    app.setWindowIcon(QIcon(":/resources/icons/logo.svg"));
    app.setApplicationDisplayName(QObject::tr("Deepin Movie"));
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);


    dmr::MainWindow mw;
    mw.setMinimumSize(QSize(528, 400));
    mw.resize(850, 600);
    DUtility::moveToCenter(&mw);
    mw.show();

    if (!toOpenFile.isEmpty()) {
        auto fi = QFileInfo(toOpenFile);
        mw.play(fi);
    }
    return app.exec();
}

