#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QShortcut>
#include <QKeySequence>
#include <QMenu>
#include <QTabBar>
#include <QContextMenuEvent>
#include <QWebEnginePage>
#include <QUrl>
#include <QScreen>
#include <QDesktopWidget>

class CustomWebEngineView : public QWebEngineView {
    Q_OBJECT
public:
    explicit CustomWebEngineView(QWidget *parent = nullptr, QMainWindow* mainWindow = nullptr)
        : QWebEngineView(parent), m_mainWindow(mainWindow) {}

protected:
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type) override {
        // Abrir enlaces con target="_blank" en nueva pestaña del mainWindow
        if(m_mainWindow) {
            return m_mainWindow->findChild<QTabWidget*>()->parentWidget()->findChild<QMainWindow*>()->findChild<QTabWidget*>()->parentWidget()->findChild<QMainWindow*>()->findChild<QTabWidget*>()->parentWidget()->findChild<QMainWindow*>()->findChild<QTabWidget*>()->parentWidget()->findChild<QMainWindow*>() ? this->m_mainWindow->findChild<QTabWidget*>()->addTab(new CustomWebEngineView(nullptr, m_mainWindow)) : nullptr;
        }
        return nullptr;
    }

private:
    QMainWindow* m_mainWindow;
};

class NovaNav : public QMainWindow {
    Q_OBJECT
public:
    explicit NovaNav(QWidget *parent = nullptr, const QUrl& initialUrl = QUrl())
        : QMainWindow(parent)
    {
        setWindowTitle("NovaNav - Super Lightweight Browser");
        resize(800, 900);

        centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        mainLayout = new QVBoxLayout(centralWidget);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setTabsClosable(true);
        mainLayout->addWidget(tabWidget);

        connect(tabWidget, &QTabWidget::tabCloseRequested, this, &NovaNav::closeTab);
        connect(tabWidget->tabBar(), &QTabBar::tabBarClicked, this, &NovaNav::addCloseButtonToTab);

        // Shortcuts
        QShortcut *shortcutNewTab = new QShortcut(QKeySequence("Ctrl+T"), this);
        connect(shortcutNewTab, &QShortcut::activated, this, &NovaNav::showUrlInputDialog);

        QShortcut *shortcutZoomIn = new QShortcut(QKeySequence("Ctrl++"), this);
        connect(shortcutZoomIn, &QShortcut::activated, this, &NovaNav::zoomIn);

        QShortcut *shortcutZoomOut = new QShortcut(QKeySequence("Ctrl+-"), this);
        connect(shortcutZoomOut, &QShortcut::activated, this, &NovaNav::zoomOut);

        QShortcut *shortcutToggleTabs = new QShortcut(QKeySequence("Ctrl+V"), this);
        connect(shortcutToggleTabs, &QShortcut::activated, this, &NovaNav::toggleTabBarVisibility);

        if (initialUrl.isValid()) {
            createNewTab(initialUrl);
        }
    }

public slots:
    void createNewTab(const QUrl& url = QUrl(), bool blank = false) {
        CustomWebEngineView* browser = new CustomWebEngineView(nullptr, this);

        if (!blank && url.isValid()) {
            browser->setUrl(url);
        }

        browser->page()->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);

        int index = tabWidget->addTab(browser, "");
        tabWidget->setCurrentIndex(index);

        connect(browser, &QWebEngineView::titleChanged, this, [this, browser, index](const QString &title) {
            tabWidget->setTabText(index, title.left(20));
        });

        connect(browser, &QWebEngineView::urlChanged, this, [this, browser, index]() {
            tabWidget->setTabText(index, browser->title().left(20));
        });

        createTabCloseButton(index);

        // Context menu support
        browser->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(browser, &QWidget::customContextMenuRequested, this, [this, browser](const QPoint &pos) {
            showCustomContextMenu(browser, pos);
        });
    }

    void closeTab(int index) {
        QWidget* widget = tabWidget->widget(index);
        if (widget) {
            tabWidget->removeTab(index);
            widget->deleteLater();
        }
    }

    void zoomIn() {
        auto browser = currentBrowser();
        if (browser)
            browser->setZoomFactor(browser->zoomFactor() + 0.1);
    }

    void zoomOut() {
        auto browser = currentBrowser();
        if (browser)
            browser->setZoomFactor(browser->zoomFactor() - 0.1);
    }

    void toggleTabBarVisibility() {
        tabWidget->tabBar()->setVisible(!tabWidget->tabBar()->isVisible());
    }

    void showUrlInputDialog() {
        // Simple input dialog implementation
        QDialog dialog(this);
        dialog.setWindowTitle("Enter URL");
        QVBoxLayout layout(&dialog);
        QLineEdit urlEdit;
        layout.addWidget(&urlEdit);
        QPushButton ok("OK");
        layout.addWidget(&ok);
        connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);
        if (dialog.exec() == QDialog::Accepted) {
            QUrl url = QUrl::fromUserInput(urlEdit.text());
            if (!url.isValid()) {
                url = QUrl("http://" + urlEdit.text());
            }
            createNewTab(url);
        }
    }

    void addCloseButtonToTab(int index) {
        if (!tabWidget->tabBar()->tabButton(index, QTabBar::RightSide)) {
            QPushButton *btnClose = new QPushButton("✖");
            btnClose->setFixedSize(10, 10);
            btnClose->setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }");
            connect(btnClose, &QPushButton::clicked, this, [this, index]() { closeTab(index); });
            tabWidget->tabBar()->setTabButton(index, QTabBar::RightSide, btnClose);
        }
    }

    void openNewWindow(const QUrl& url) {
        QMainWindow* newWindow = new QMainWindow();
        newWindow->setWindowTitle("NovaNav - New Window");
        newWindow->resize(800, 900);

        // Center the window on the screen
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        int x = (screenGeometry.width() - newWindow->width()) / 2;
        int y = (screenGeometry.height() - newWindow->height()) / 2;
        newWindow->move(x, y);

        NovaNav* newNav = new NovaNav(nullptr);
        newWindow->setCentralWidget(newNav);
        newNav->show();
        newNav->createNewTab(url);

        // Keep reference so it is not deleted
        open_windows.append(newWindow);

        newWindow->show();
    }

private:
    CustomWebEngineView* currentBrowser() const {
        return qobject_cast<CustomWebEngineView*>(tabWidget->currentWidget());
    }

    void createTabCloseButton(int index) {
        QPushButton* btnClose = new QPushButton("✖");
        btnClose->setFixedSize(10, 10);
        btnClose->setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }");
        connect(btnClose, &QPushButton::clicked, this, [this, index]() { closeTab(index); });
        tabWidget->tabBar()->setTabButton(index, QTabBar::RightSide, btnClose);
    }

    void showCustomContextMenu(CustomWebEngineView* browser, const QPoint& pos) {
        QMenu menu;

        QAction* newTab = menu.addAction("Open link in new tab");
        QAction* newWindow = menu.addAction("Open link in new window");

        QAction* back = menu.addAction("Back");
        QAction* forward = menu.addAction("Forward");
        QAction* refresh = menu.addAction("Refresh");
        QAction* toggleTabs = menu.addAction("Show/Hide Tabs");
        QAction* credits = menu.addAction("Credits");

        QUrl linkUrl = browser->page()->contextMenuData().linkUrl();

        if (!linkUrl.isEmpty()) {
            connect(newTab, &QAction::triggered, this, [this, linkUrl]() { createNewTab(linkUrl); });
            connect(newWindow, &QAction::triggered, this, [this, linkUrl]() { openNewWindow(linkUrl); });
        } else {
            newTab->setEnabled(false);
            newWindow->setEnabled(false);
        }

        connect(back, &QAction::triggered, browser, &QWebEngineView::back);
        connect(forward, &QAction::triggered, browser, &QWebEngineView::forward);
        connect(refresh, &QAction::triggered, browser, &QWebEngineView::reload);
        connect(toggleTabs, &QAction::triggered, this, &NovaNav::toggleTabBarVisibility);
        connect(credits, &QAction::triggered, this, &NovaNav::showCreditsPopup);

        menu.exec(browser->mapToGlobal(pos));
    }

    void showCreditsPopup() {
        QDialog creditsPopup(this);
        creditsPopup.setWindowTitle("Credits");
        QVBoxLayout layout(&creditsPopup);
        QLabel label(
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
            "For inquiries about usage restrictions, contact:\n"
            "Felipe Alfonso González\n"
            "Email: f.alfonso@res-ear.ch\n"
            "\n"
            "--------------------------------------------------\n"
            "FAQ:\n"
            "\n"
            "ctrl+t (New tab)\n"
            "ctrl+v (hide tabs for distraction-free)\n"
            "ctrl+q (quit)\n"
        );
        label.setAlignment(Qt::AlignCenter);
        layout.addWidget(&label);
        creditsPopup.exec();
    }

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QTabWidget* tabWidget;
    URLInputDialog* url_input_dialog;

    QList<QMainWindow*> open_windows;
};


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    NovaNav w(nullptr, QUrl("https://www.google.com"));
    w.show();
    return a.exec();
}

#include "main.moc"
