#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QShortcut>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineSettings>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QTabBar>
#include <QMenu>
#include <QtWebEngineWidgets/QWebEngineHistory> // Agregar esta línea
#include <QtWebEngineWidgets/QWebEngineContextMenuData> // Agregar esta línea
#include <QScreen>

class URLInputDialog : public QDialog
{
public:
    URLInputDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("Enter URL");
        QVBoxLayout *layout = new QVBoxLayout(this);
        url_entry = new QLineEdit();
        layout->addWidget(url_entry);
        ok_button = new QPushButton("OK");
        layout->addWidget(ok_button);
        connect(ok_button, &QPushButton::clicked, this, &QDialog::accept);
    }

    QLineEdit *url_entry;
    QPushButton *ok_button;
};

class NovaNav : public QMainWindow
{
    Q_OBJECT

public:
    NovaNav(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("NovaNav - Super Lightweight Browser");

        setGeometry(100, 100, 800, 900);

        central_widget = new QWidget();
        setCentralWidget(central_widget);
        layout = new QVBoxLayout(central_widget);

        tab_widget = new QTabWidget();
        tab_widget->setTabsClosable(true);
        layout->addWidget(tab_widget);

        url_input_dialog = new URLInputDialog(this);

        shortcut_new_tab = new QShortcut(QKeySequence("Ctrl+T"), this);
        connect(shortcut_new_tab, &QShortcut::activated, this, &NovaNav::show_url_input_dialog);

        shortcut_zoom_in = new QShortcut(Qt::CTRL + Qt::Key_Plus, this);
        connect(shortcut_zoom_in, &QShortcut::activated, this, &NovaNav::zoom_in);

        shortcut_zoom_out = new QShortcut(Qt::CTRL + Qt::Key_Minus, this);
        connect(shortcut_zoom_out, &QShortcut::activated, this, &NovaNav::zoom_out);

        shortcut_toggle_titles = new QShortcut(Qt::CTRL + Qt::Key_V, this);
        connect(shortcut_toggle_titles, &QShortcut::activated, this, &NovaNav::toggle_titles);

        create_new_tab("https://www.google.com");

        add_navigation_buttons(); // Agregar botones de navegación

        connect(tab_widget, &QTabWidget::tabCloseRequested, this, &NovaNav::close_tab);

            QShortcut *shortcut_quit = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this);
    connect(shortcut_quit, &QShortcut::activated, qApp, &QApplication::quit);
    

// Conectar la señal tabBarClicked del QTabWidget a una ranura para agregar botones de cierre personalizados
        connect(tab_widget, &QTabWidget::tabBarClicked, this, &NovaNav::addCloseButtonToTab);



        // Set permissions and settings...
    }

public slots:
    void show_url_input_dialog()
 
{
    if (url_input_dialog->exec() == QDialog::Accepted)
    {
        QString url = url_input_dialog->url_entry->text();
        // Verificar si la URL comienza con "http://" o "https://"
        if (!url.startsWith("http://") && !url.startsWith("https://"))
        {
            // Si no comienza con "http://" o "https://", agregar "http://"
            url = "http://" + url;
        }
        create_new_tab(url);
    }
}


    void close_tab(int index)
    {
        QWidget *widgetToRemove = tab_widget->widget(index);
        QWebEngineView *browserToRemove = dynamic_cast<QWebEngineView *>(widgetToRemove);

        if (browserToRemove)
        {
            // Desconectar todas las señales conectadas a la página del motor web
            browserToRemove->page()->disconnect();
            // Eliminar la página del motor web
            delete browserToRemove->page();
        }

        tab_widget->removeTab(index);
    }

    void zoom_in()
    {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser)
            current_browser->setZoomFactor(current_browser->zoomFactor() + 0.1);
    }

    void zoom_out()
    {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser)
            current_browser->setZoomFactor(current_browser->zoomFactor() - 0.1);
    }

    void toggle_titles()
    {
        for (int i = 0; i < tab_widget->count(); ++i)
        {
            QWebEngineView *browser = dynamic_cast<QWebEngineView *>(tab_widget->widget(i));
            QString title = browser->title();
            if (!tab_widget->tabBar()->isVisible())
                title = tab_widget->tabText(i);
            tab_widget->setTabText(i, title.left(20));
        }
        tab_widget->tabBar()->setVisible(!tab_widget->tabBar()->isVisible());
    }

private:
    QWidget *central_widget;
    QVBoxLayout *layout;
    QTabWidget *tab_widget;
    URLInputDialog *url_input_dialog;
    QShortcut *shortcut_new_tab;
    QShortcut *shortcut_zoom_in;
    QShortcut *shortcut_zoom_out;
    QShortcut *shortcut_toggle_titles;

    // Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36
    // browser->page()->profile()->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");
    // browser->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36");
    //  browser->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36");

    // sqDebug() << "User-Agent:" << browser->page()->profile()->httpUserAgent();
    // profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

void create_new_tab(const QString &url)
{
    // Crear un identificador único para el perfil
    static int profileCount = 0;
    QString profileName = "CustomProfile_" + QString::number(profileCount++);

    // Crear un nuevo perfil de navegación
    QWebEngineProfile *profile = new QWebEngineProfile(profileName);

    // Modificar el User-Agent en ciertos sitios web
    profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

    // Crear un QWebEngineView y establecer la URL
    QWebEngineView *browser = new QWebEngineView();
    browser->setUrl(QUrl(url)); // Establecer la URL proporcionada

    // Permitir JavaScript
    browser->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    // Conectar señales de cambio de título y URL
    connect(browser, &QWebEngineView::titleChanged, [=](const QString &title)
            { set_tab_title(browser, title.left(20)); });

    // Conectar la señal urlChanged para detectar cuando se cambia la URL
    connect(browser, &QWebEngineView::urlChanged, [=](const QUrl &newUrl)
    {
        if (newUrl.toString().endsWith("_blank")) {
            create_new_tab(newUrl.toString()); // Abrir enlace en nueva pestaña
        }
    });

    // Agregar el QWebEngineView a la pestaña
    tab_widget->addTab(browser, "");
    browser->setZoomFactor(0.67); // Zoom por defecto

    // Crear el botón de cierre personalizado
    QPushButton *closeButton = new QPushButton("✖");
    closeButton->setFixedSize(10, 10);
    closeButton->setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }");

    // Conectar la señal clicked del botón a la ranura para cerrar la pestaña
    connect(closeButton, &QPushButton::clicked, [=]() {
        close_tab(tab_widget->indexOf(browser)); // Cerrar la pestaña que contiene este navegador
    });

    // Establecer el botón de cierre personalizado en la pestaña recién creada
    tab_widget->tabBar()->setTabButton(tab_widget->count() - 1, QTabBar::RightSide, closeButton);

    // Configurar el menú contextual para abrir enlaces en nueva pestaña o ventana
    browser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(browser, &QWebEngineView::customContextMenuRequested, this, &NovaNav::onCustomContextMenuRequested);
}





// Función para agregar botones de cierre personalizados a las pestañas
    void addCloseButtonToTab(int tabIndex)
    {
        // Si no hay ningún botón en el área de cierre de la pestaña actual, agrega uno
        if (!tab_widget->tabBar()->tabButton(tabIndex, QTabBar::RightSide)) {
            QPushButton *close_button = new QPushButton("✖");
            close_button->setFixedSize(10, 10);
            close_button->setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }");

            // Conecta la señal clicked del botón a la ranura para cerrar la pestaña
            connect(close_button, &QPushButton::clicked, [=]() {
                // Lógica para cerrar la pestaña
                close_tab(tabIndex);
            });

            tab_widget->tabBar()->setTabButton(tabIndex, QTabBar::RightSide, close_button);
        }
    }

    // Función para crear y agregar una nueva pestaña
    void createAndAddNewTab(const QString &url)
    {
        create_new_tab(url);
        // Agregar botones de cierre personalizados a la nueva pestaña
        addCloseButtonToTab(tab_widget->count() - 1);
    }





    void set_tab_title(QWebEngineView *browser, const QString &title)
    {
        int index = tab_widget->indexOf(browser);
        tab_widget->setTabText(index, title);
    }

    void add_navigation_buttons()
    {
        QHBoxLayout *navigation_layout = new QHBoxLayout();

        QPushButton *plus_button = new QPushButton("+");
        plus_button->setFixedSize(10, 10); // Tamaño más pequeño
        connect(plus_button, &QPushButton::clicked, this, &NovaNav::open_google_tab);
        navigation_layout->addWidget(plus_button);

        QPushButton *backward_button = new QPushButton("<");
        backward_button->setFixedSize(10, 10); // Tamaño más pequeño
        connect(backward_button, &QPushButton::clicked, this, &NovaNav::go_back);
        navigation_layout->addWidget(backward_button);

        QPushButton *forward_button = new QPushButton(">");
        forward_button->setFixedSize(10, 10); // Tamaño más pequeño
        connect(forward_button, &QPushButton::clicked, this, &NovaNav::go_forward);
        navigation_layout->addWidget(forward_button);

        QPushButton *new_tab_button = new QPushButton("o");
        new_tab_button->setFixedSize(10, 10); // Tamaño más pequeño
        connect(new_tab_button, &QPushButton::clicked, this, &NovaNav::show_url_input_dialog);
        navigation_layout->addWidget(new_tab_button);

        QPushButton *credits_button = new QPushButton("=");
        credits_button->setFixedSize(10, 10); // Tamaño más pequeño
        connect(credits_button, &QPushButton::clicked, this, &NovaNav::show_credits_popup);
        navigation_layout->addWidget(credits_button);

        QPushButton *ctrl_v_button = new QPushButton("-");
        ctrl_v_button->setFixedSize(10, 10); // Tamaño más pequeño
        connect(ctrl_v_button, &QPushButton::clicked, this, &NovaNav::toggle_titles);
        navigation_layout->addWidget(ctrl_v_button);

        navigation_layout->addStretch();

        QWidget *navigation_container = new QWidget();
        navigation_container->setLayout(navigation_layout);

        tab_widget->setCornerWidget(navigation_container, Qt::TopRightCorner);

// Itera sobre las pestañas y agrega el botón personalizado en el área de cierre de cada una
// Oculta los botones de cierre predeterminados en todas las pestañas
for (int i = 0; i < tab_widget->count(); ++i) {
    tab_widget->tabBar()->tabButton(i, QTabBar::RightSide)->hide();
}

// Itera sobre las pestañas y agrega el botón personalizado en el área de cierre de cada una
for (int i = 0; i < tab_widget->count(); ++i) {
    QPushButton *close_button = new QPushButton("✖");
    close_button->setFixedSize(10, 10);
    close_button->setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }");

    // Conecta la señal clicked del botón a la ranura para cerrar la pestaña
    connect(close_button, &QPushButton::clicked, [=]() {
        // Lógica para cerrar la pestaña
        close_tab(i);
    });

    tab_widget->tabBar()->setTabButton(i, QTabBar::RightSide, close_button);
}





    }

    void open_google_tab()
    {
        create_new_tab("https://www.google.com");
    }

    void show_credits_popup()
    {
        QDialog *credits_popup = new QDialog(this);
        credits_popup->setWindowTitle("Credits");

        QVBoxLayout *credits_layout = new QVBoxLayout(credits_popup);

        QLabel *credits_label = new QLabel(
            "NovaNav - Super Lightweight Browser\n\n"
            "Credits:\n"
            "Computer Science Engineer: Felipe Alfonso González\n"
            "GitHub: github.com/felipealfonsog\n"
            "\n"
            "--------------------------------------------------\n"
            "This software is licensed under the BSD 3-Clause License.\n"
            "For details, see: https://opensource.org/licenses/BSD-3-Clause\n"
            "\n"
            "Usage restrictions:\n"
            "For any inquiries about usage restrictions, please contact:\n"
            "Felipe Alfonso González\n"
            "Email: f.alfonso@res-ear.ch\n"
            "\n"
            "--------------------------------------------------\n"
            "FAQ:\n"
            "\n"
            "ctrl+t (New tab)\n"
            "ctrl+v (hide tabs for distraction-free)\n"
            "ctrl+q (quit)\n"
            "\n");

        credits_label->setAlignment(Qt::AlignCenter);
        credits_layout->addWidget(credits_label);

        credits_popup->exec();
    }

    void go_back()
    {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser)
        {
            if (current_browser->history()->canGoBack())
                current_browser->back();
        }
    }

    void go_forward()
    {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser)
        {
            if (current_browser->history()->canGoForward())
                current_browser->forward();
        }
    }


void open_new_window(const QUrl& url)
{
    QWebEngineView *newBrowser = new QWebEngineView();
    newBrowser->setUrl(url);
    newBrowser->setZoomFactor(0.67); // Zoom al 67%
    newBrowser->show();

    // Conectar el menú contextual para el nuevo navegador
    newBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(newBrowser, &QWebEngineView::customContextMenuRequested, this, &NovaNav::onCustomContextMenuRequestedForNewWindow);

    // Centrar la ventana en la pantalla
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - newBrowser->width()) / 2;
    int y = (screenGeometry.height() - newBrowser->height()) / 2;
    newBrowser->move(x, y);

    // Establecer dimensiones de la ventana
    newBrowser->resize(800, 500);
}

void onCustomContextMenuRequestedForNewWindow(const QPoint &pos)
{
    QWebEngineView *newBrowser = dynamic_cast<QWebEngineView *>(sender());
    if (!newBrowser)
        return;

    QMenu menu;

    QAction *backAction = menu.addAction(tr("Back"));
    QAction *forwardAction = menu.addAction(tr("Forward"));
    QAction *refreshAction = menu.addAction(tr("Refresh"));
    QAction *creditsAction = menu.addAction(tr("Credits"));

    connect(backAction, &QAction::triggered, [=]() {
        if (newBrowser->history()->canGoBack())
            newBrowser->back();
    });

    connect(forwardAction, &QAction::triggered, [=]() {
        if (newBrowser->history()->canGoForward())
            newBrowser->forward();
    });

    connect(refreshAction, &QAction::triggered, [=]() {
        newBrowser->reload();
    });

    connect(creditsAction, &QAction::triggered, this, &NovaNav::show_credits_popup);

    menu.exec(newBrowser->mapToGlobal(pos));
}


    void onCustomContextMenuRequested(const QPoint &pos)
{
    QWebEngineView *browser = dynamic_cast<QWebEngineView *>(sender());
    if (!browser)
        return;

    QMenu menu;

    QAction *newTabAction = menu.addAction(tr("Open link in new tab"));
    QAction *newWindowAction = menu.addAction(tr("Open link in new window"));
    QAction *backAction = menu.addAction(tr("Back"));
    QAction *forwardAction = menu.addAction(tr("Forward"));
    QAction *refreshAction = menu.addAction(tr("Refresh"));
    QAction *showTabsAction = menu.addAction(tr("Show/Hide Tabs"));
    QAction *creditsAction = menu.addAction(tr("Credits"));

    QUrl linkUrl = browser->page()->contextMenuData().linkUrl();
    if (!linkUrl.isEmpty()) {
        connect(newTabAction, &QAction::triggered, [=]() {
            create_new_tab(linkUrl.toString());
        });

        connect(newWindowAction, &QAction::triggered, [=]() {
            open_new_window(linkUrl);
        });
    } else {
        newTabAction->setEnabled(false);
        newWindowAction->setEnabled(false);
    }

    connect(backAction, &QAction::triggered, [=]() {
        if (browser->history()->canGoBack())
            browser->back();
    });

    connect(forwardAction, &QAction::triggered, [=]() {
        if (browser->history()->canGoForward())
            browser->forward();
    });

    connect(refreshAction, &QAction::triggered, [=]() {
        browser->reload();
    });

    connect(showTabsAction, &QAction::triggered, this, &NovaNav::toggle_titles);

    connect(creditsAction, &QAction::triggered, this, &NovaNav::show_credits_popup);

    menu.exec(browser->mapToGlobal(pos));
}


};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    NovaNav window;
    window.show();
    return app.exec();
}

#include "novanav.moc"
