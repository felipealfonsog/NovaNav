void create_new_tab(const QString &url) {
 
    // Crear un nuevo perfil de navegación
    QWebEngineProfile *profile = new QWebEngineProfile("CustomProfile");

    // Modificar el User-Agent en ciertos sitios web
    profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

    // Crear un QWebEngineView y establecer la URL
    QWebEngineView *browser = new QWebEngineView();
    if (!url.startsWith("http"))
        browser->setUrl(QUrl("http://" + url));
    else
        browser->setUrl(QUrl(url));

    // Permitir JavaScript
    browser->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    // Conectar señales de cambio de título y URL
    connect(browser, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        set_tab_title(browser, title.left(20));
    });

    // Conectar la señal de cambio de URL para abrir enlaces _blank en una nueva pestaña
    connect(browser, &QWebEngineView::urlChanged, this, [=](const QUrl &newUrl) {
        // Verificar si el enlace es _blank
        if (newUrl.toString().contains("_blank")) {
            create_new_tab(newUrl.toString()); // Abrir enlace en nueva pestaña
        }
    });

    // Agregar el QWebEngineView a la pestaña
    tab_widget->addTab(browser, "");
    browser->setZoomFactor(0.67); // Zoom por defecto

    // Configurar el menú contextual para abrir enlaces en nueva pestaña o ventana
    browser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(browser, &QWebEngineView::customContextMenuRequested, this, [=](const QPoint &pos) {
        // Crear el menú contextual
        QMenu contextMenu(tr("Context menu"), this);
        // Agregar acciones al menú
        QAction *newTabAction = contextMenu.addAction(tr("Open link in new tab"));
        QAction *newWindowAction = contextMenu.addAction(tr("Open link in new window"));
        // Mostrar el menú contextual y esperar a que se seleccione una acción
        QAction *selectedAction = contextMenu.exec(browser->mapToGlobal(pos));
        // Procesar la acción seleccionada
        if (selectedAction == newTabAction) {
            create_new_tab(browser->url().toString()); // Abrir enlace en nueva pestaña
        } else if (selectedAction == newWindowAction) {
            QWebEngineView *newBrowser = new QWebEngineView();
            newBrowser->setUrl(browser->url());
            newBrowser->show(); // Abrir enlace en nueva ventana
        }
    });
}



// ---- v2

void create_new_tab(const QString &url) {
    // Crear un nuevo perfil de navegación
    QWebEngineProfile *profile = new QWebEngineProfile("CustomProfile");

    // Modificar el User-Agent en ciertos sitios web
    profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

    // Crear un QWebEngineView y establecer la URL
    QWebEngineView *browser = new QWebEngineView();
    if (!url.startsWith("http"))
        browser->setUrl(QUrl("http://" + url));
    else
        browser->setUrl(QUrl(url));

    // Permitir JavaScript
    browser->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    // Conectar señales de cambio de título y URL
    connect(browser, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        set_tab_title(browser, title.left(20));
    });

    // Conectar la señal de cambio de URL para abrir enlaces _blank en una nueva pestaña
    connect(browser, &QWebEngineView::urlChanged, this, [=](const QUrl &newUrl) {
        // Verificar si el enlace es _blank
        if (newUrl.toString().contains("_blank")) {
            create_new_tab(newUrl.toString()); // Abrir enlace en nueva pestaña
        }
    });

    // Agregar el QWebEngineView a la pestaña
    tab_widget->addTab(browser, "");
    browser->setZoomFactor(0.67); // Zoom por defecto

    // Configurar el menú contextual para abrir enlaces en nueva pestaña o ventana
    browser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(browser, &QWebEngineView::customContextMenuRequested, this, [=](const QPoint &pos) {
        // Crear el menú contextual
        QMenu contextMenu(tr("Context menu"), this);
        // Agregar acciones al menú
        QAction *newTabAction = contextMenu.addAction(tr("Open link in new tab"));
        QAction *newWindowAction = contextMenu.addAction(tr("Open link in new window"));
        QAction *refreshAction = contextMenu.addAction(tr("Refresh"));
        // Mostrar el menú contextual y esperar a que se seleccione una acción
        QAction *selectedAction = contextMenu.exec(browser->mapToGlobal(pos));
        // Procesar la acción seleccionada
        if (selectedAction == newTabAction) {
            create_new_tab(browser->url().toString()); // Abrir enlace en nueva pestaña
        } else if (selectedAction == newWindowAction) {
            QWebEngineView *newBrowser = new QWebEngineView();
            newBrowser->setUrl(browser->url());
            newBrowser->show(); // Abrir enlace en nueva ventana
        } else if (selectedAction == refreshAction) {
            browser->reload(); // Actualizar página
        }
    });
}


// v3





void create_new_tab(const QString &url) {
    // Crear un nuevo perfil de navegación
    QWebEngineProfile *profile = new QWebEngineProfile("CustomProfile");

    // Modificar el User-Agent en ciertos sitios web
    profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

    // Crear un QWebEngineView y establecer la URL
    QWebEngineView *browser = new QWebEngineView();
    if (!url.startsWith("http"))
        browser->setUrl(QUrl("http://" + url));
    else
        browser->setUrl(QUrl(url));

    // Permitir JavaScript
    browser->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    // Conectar señales de cambio de título y URL
    connect(browser, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        set_tab_title(browser, title.left(20));
    });

    // Conectar la señal de cambio de URL para abrir enlaces _blank en una nueva pestaña
    connect(browser, &QWebEngineView::urlChanged, this, [=](const QUrl &newUrl) {
        // Verificar si el enlace es _blank
        if (newUrl.toString().contains("_blank")) {
            create_new_tab(newUrl.toString()); // Abrir enlace en nueva pestaña
        }
    });

    // Agregar el QWebEngineView a la pestaña
    tab_widget->addTab(browser, "");
    browser->setZoomFactor(0.67); // Zoom por defecto

    // Configurar el menú contextual para abrir enlaces en nueva pestaña o ventana
    browser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(browser, &QWebEngineView::customContextMenuRequested, this, [=](const QPoint &pos) {
        // Crear el menú contextual
        QMenu contextMenu(tr("Context menu"), this);
        // Agregar acciones al menú
        QAction *newTabAction = contextMenu.addAction(tr("Open link in new tab"));
        QAction *newWindowAction = contextMenu.addAction(tr("Open link in new window"));
        QAction *refreshAction = contextMenu.addAction(tr("Refresh"));
        // Mostrar el menú contextual y esperar a que se seleccione una acción
        QAction *selectedAction = contextMenu.exec(browser->mapToGlobal(pos));
        // Procesar la acción seleccionada
        if (selectedAction == newTabAction) {
            create_new_tab(browser->url().toString()); // Abrir enlace en nueva pestaña
        } else if (selectedAction == newWindowAction) {
            open_new_window(browser->url().toString()); // Abrir enlace en nueva ventana
        } else if (selectedAction == refreshAction) {
            browser->reload(); // Recargar la página
        }
    });
}

void open_new_window(const QString &url) {
    QWebEngineView *newBrowser = new QWebEngineView();
    if (!url.startsWith("http"))
        newBrowser->setUrl(QUrl("http://" + url));
    else
        newBrowser->setUrl(QUrl(url));
    newBrowser->show(); // Abrir enlace en nueva ventana
}




