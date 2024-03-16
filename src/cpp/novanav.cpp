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
#include <QtWebEngineWidgets/QWebEngineHistory> // Agregar esta línea

class URLInputDialog : public QDialog {
public:
    URLInputDialog(QWidget *parent = nullptr) : QDialog(parent) {
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

class NovaNav : public QMainWindow {
    Q_OBJECT

public:
    NovaNav(QWidget *parent = nullptr) : QMainWindow(parent) {
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

        add_navigation_buttons();  // Agregar botones de navegación

        connect(tab_widget, &QTabWidget::tabCloseRequested, this, &NovaNav::close_tab);


        // Set permissions and settings...
    }

public slots:
void show_url_input_dialog() {

    if (url_input_dialog->exec() == QDialog::Accepted) {
        QString url = url_input_dialog->url_entry->text();
        create_new_tab(url);
    }
}


    void close_tab(int index) {
        tab_widget->removeTab(index);
    }

    void zoom_in() {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser)
            current_browser->setZoomFactor(current_browser->zoomFactor() + 0.1);
    }

    void zoom_out() {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser)
            current_browser->setZoomFactor(current_browser->zoomFactor() - 0.1);
    }

    void toggle_titles() {
        for (int i = 0; i < tab_widget->count(); ++i) {
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


void create_new_tab(const QString &url) {
    // Creamos un nuevo perfil de navegación
    QWebEngineProfile *profile = new QWebEngineProfile("CustomProfile");

    // Modificamos el User-Agent en ciertos sitios web
    profile->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");

// Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36
// browser->page()->profile()->setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36");
   // browser->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36");
   //  browser->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36");


    // sqDebug() << "User-Agent:" << browser->page()->profile()->httpUserAgent();



    // Creamos el QWebEngineView y establecemos la URL
    QWebEngineView *browser = new QWebEngineView();
    if (!url.startsWith("http"))
        browser->setUrl(QUrl("http://" + url));
    else
        browser->setUrl(QUrl(url));

    // Habilitamos JavaScript
    browser->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

    // Conectamos señales de cambio de título y URL
    connect(browser, &QWebEngineView::titleChanged, this, [=](const QString &title) {
        set_tab_title(browser, title.left(20));
    });

    connect(browser, &QWebEngineView::urlChanged, this, [=](const QUrl &newUrl) {
        // Verificar si el enlace _blank
        if (newUrl.toString().contains("_blank")) {
            create_new_tab(newUrl.toString()); // Abrir enlace en nueva pestaña
        }
    });

    // Agregamos el QWebEngineView a la pestaña
    tab_widget->addTab(browser, "");
    browser->setZoomFactor(0.67); // Zoom por defecto
}




    void set_tab_title(QWebEngineView *browser, const QString &title) {
        int index = tab_widget->indexOf(browser);
        tab_widget->setTabText(index, title);
    }

    void add_navigation_buttons() {
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
    }

    void open_google_tab() {
        create_new_tab("https://www.google.com");
    }

    void show_credits_popup() {
        QDialog *credits_popup = new QDialog(this);
        credits_popup->setWindowTitle("Credits");

        QVBoxLayout *credits_layout = new QVBoxLayout(credits_popup);

        QLabel *credits_label = new QLabel(
            "Credits:\n"
            "Engineer: Felipe Alfonso Gonzalez\n"
            "GitHub: github.com/felipealfonsog\n"
            "\n"
            "This software is licensed under the BSD 3-Clause License.\n"
            "For details, see: https://opensource.org/licenses/BSD-3-Clause\n"
            "\n"
            "Usage restrictions:\n"
            "For any inquiries about usage restrictions, please contact:\n"
            "Felipe Alfonso Gonzalez\n"
            "Email: f.alfonso@res-ear.ch\n"
        );

        credits_label->setAlignment(Qt::AlignCenter);
        credits_layout->addWidget(credits_label);

        credits_popup->exec();
    }

    void go_back() {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser) {
            if (current_browser->history()->canGoBack())
                current_browser->back();
        }
    }

    void go_forward() {
        QWebEngineView *current_browser = dynamic_cast<QWebEngineView *>(tab_widget->currentWidget());
        if (current_browser) {
            if (current_browser->history()->canGoForward())
                current_browser->forward();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    NovaNav window;
    window.show();
    return app.exec();
}

#include "novanav.moc"
