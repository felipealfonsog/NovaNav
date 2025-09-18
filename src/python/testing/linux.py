import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QTabWidget, QLineEdit, QPushButton, QDialog, QHBoxLayout, QLabel, QShortcut, QMenu, QTabBar
from PyQt5.QtGui import QKeySequence
from PyQt5.QtCore import Qt, QUrl
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEngineProfile, QWebEngineSettings


class URLInputDialog(QDialog):
    def __init__(self, parent=None):
        super(URLInputDialog, self).__init__(parent)
        self.setWindowTitle("Enter URL")
        layout = QVBoxLayout(self)
        self.url_entry = QLineEdit()
        layout.addWidget(self.url_entry)
        self.ok_button = QPushButton("OK")
        layout.addWidget(self.ok_button)
        self.ok_button.clicked.connect(self.accept)


class CustomWebEngineView(QWebEngineView):
    def __init__(self, main_window, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.main_window = main_window

    def createWindow(self, windowType):
        # Abrir links con target=_blank en una nueva pestaña
        return self.main_window.create_new_tab(blank=True)


class NovaNav(QMainWindow):
    def __init__(self, parent=None, initial_url=None):
        super(NovaNav, self).__init__(parent)
        self.setWindowTitle("NovaNav - Super Lightweight Browser")
        self.setGeometry(100, 100, 800, 900)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.tab_widget = QTabWidget()
        self.tab_widget.setTabsClosable(True)
        self.layout.addWidget(self.tab_widget)

        self.url_input_dialog = URLInputDialog(self)

        self.shortcut_new_tab = QShortcut(QKeySequence("Ctrl+T"), self)
        self.shortcut_new_tab.activated.connect(self.show_url_input_dialog)

        self.shortcut_zoom_in = QShortcut(Qt.CTRL + Qt.Key_Plus, self)
        self.shortcut_zoom_in.activated.connect(self.zoom_in)

        self.shortcut_zoom_out = QShortcut(Qt.CTRL + Qt.Key_Minus, self)
        self.shortcut_zoom_out.activated.connect(self.zoom_out)

        self.shortcut_toggle_titles = QShortcut(Qt.CTRL + Qt.Key_V, self)
        self.shortcut_toggle_titles.activated.connect(self.toggle_titles)

        # Mantener referencias de ventanas nuevas para evitar cierre prematuro
        self.open_windows = []

        # Solo crear pestaña inicial si se dio URL
        if initial_url is not None:
            self.create_new_tab(initial_url)

        self.add_navigation_buttons()
        self.tab_widget.tabCloseRequested.connect(self.close_tab)
        shortcut_quit = QShortcut(QKeySequence(Qt.CTRL + Qt.Key_Q), self)
        shortcut_quit.activated.connect(QApplication.quit)
        self.tab_widget.tabBarClicked.connect(self.addCloseButtonToTab)

    def set_tab_title(self, index, title):
        title = title[:20] if len(title) > 20 else title
        self.tab_widget.setTabText(index, title)

    def show_url_input_dialog(self):
        if self.url_input_dialog.exec_() == QDialog.Accepted:
            url = self.url_input_dialog.url_entry.text()
            if not url.startswith(("http://", "https://")):
                url = "http://" + url
            self.create_new_tab(url)

    def close_tab(self, index):
        widget_to_remove = self.tab_widget.widget(index)
        browser_to_remove = widget_to_remove if isinstance(widget_to_remove, QWebEngineView) else widget_to_remove.findChild(QWebEngineView)
        if browser_to_remove:
            browser_to_remove.page().disconnect()
            browser_to_remove.close()
        self.tab_widget.removeTab(index)

    def zoom_in(self):
        current_browser = self.tab_widget.currentWidget()
        if isinstance(current_browser, QWebEngineView):
            current_browser.setZoomFactor(current_browser.zoomFactor() + 0.1)

    def zoom_out(self):
        current_browser = self.tab_widget.currentWidget()
        if isinstance(current_browser, QWebEngineView):
            current_browser.setZoomFactor(current_browser.zoomFactor() - 0.1)

    def toggle_titles(self):
        for i in range(self.tab_widget.count()):
            browser = self.tab_widget.widget(i)
            if isinstance(browser, QWebEngineView):
                # Actualizar siempre el título según título actual
                self.tab_widget.setTabText(i, browser.title())
        self.tab_widget.tabBar().setVisible(not self.tab_widget.tabBar().isVisible())

    def create_new_tab(self, url=None, blank=False):
        profile_name = f"CustomProfile_{len(self.tab_widget)}"
        profile = QWebEngineProfile(profile_name)
        profile.setHttpUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, como Gecko) Chrome/122.0.0.0 Safari/537.36")

        browser = CustomWebEngineView(self)
        if not blank and url:
            browser.setUrl(QUrl(url))

        browser.page().settings().setAttribute(QWebEngineSettings.JavascriptEnabled, True)
        index = self.tab_widget.addTab(browser, "")
        self.tab_widget.setCurrentIndex(index)

        # Actualizar título en estas señales para asegurar sincronización completa
        browser.loadFinished.connect(lambda: self.set_tab_title(index, browser.title()))
        browser.titleChanged.connect(lambda title, i=index: self.set_tab_title(i, title))
        browser.urlChanged.connect(lambda url, i=index: self.set_tab_title(i, browser.title()))
        browser.setZoomFactor(0.67)

        close_button = QPushButton("✖")
        close_button.setFixedSize(10, 10)
        close_button.setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }")
        close_button.clicked.connect(lambda: self.close_tab(self.tab_widget.indexOf(browser)))
        self.tab_widget.tabBar().setTabButton(self.tab_widget.count() - 1, QTabBar.RightSide, close_button)

        browser.setContextMenuPolicy(Qt.CustomContextMenu)
        browser.customContextMenuRequested.connect(lambda pos, browser=browser: self.onCustomContextMenuRequested(pos, browser))

        return browser if blank else None

    def addCloseButtonToTab(self, tabIndex):
        if not self.tab_widget.tabBar().tabButton(tabIndex, QTabBar.RightSide):
            close_button = QPushButton("✖")
            close_button.setFixedSize(10, 10)
            close_button.setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 10px; color: black; }")
            close_button.clicked.connect(lambda: self.close_tab(tabIndex))
            self.tab_widget.tabBar().setTabButton(tabIndex, QTabBar.RightSide, close_button)

    def add_navigation_buttons(self):
        navigation_layout = QHBoxLayout()
        buttons = [
            ("+", self.open_google_tab),
            ("<", self.go_back),
            (">", self.go_forward),
            ("o", self.show_url_input_dialog),
            ("=", self.show_credits_popup),
            ("-", self.toggle_titles)
        ]
        for text, action in buttons:
            button = QPushButton(text)
            button.setFixedSize(10, 10)
            button.clicked.connect(action)
            navigation_layout.addWidget(button)
        navigation_layout.addStretch()
        navigation_container = QWidget()
        navigation_container.setLayout(navigation_layout)
        self.tab_widget.setCornerWidget(navigation_container, Qt.TopRightCorner)
        for i in range(self.tab_widget.count()):
            self.addCloseButtonToTab(i)

    def open_google_tab(self):
        self.create_new_tab("https://www.google.com")

    def show_credits_popup(self):
        credits_popup = QDialog(self)
        credits_popup.setWindowTitle("Credits")
        credits_layout = QVBoxLayout(credits_popup)
        credits_label = QLabel(
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
            "\n"
        )
        credits_label.setAlignment(Qt.AlignCenter)
        credits_layout.addWidget(credits_label)
        credits_popup.exec_()

    def go_back(self):
        current_browser = self.tab_widget.currentWidget()
        if isinstance(current_browser, QWebEngineView) and current_browser.history().canGoBack():
            current_browser.back()

    def go_forward(self):
        current_browser = self.tab_widget.currentWidget()
        if isinstance(current_browser, QWebEngineView) and current_browser.history().canGoForward():
            current_browser.forward()

    def open_new_window(self, url):
        new_window = QMainWindow()
        new_window.setWindowTitle("NovaNav - New Window")
        new_window.resize(800, 900)
        screen_geometry = QApplication.primaryScreen().geometry()
        x = (screen_geometry.width() - new_window.width()) / 2
        y = (screen_geometry.height() - new_window.height()) / 2
        new_window.move(int(x), int(y))

        new_nav = NovaNav(parent=new_window)
        new_window.setCentralWidget(new_nav)
        new_nav.show()

        new_nav.create_new_tab(url)

        self.open_windows.append(new_window)

   

    def onCustomContextMenuRequestedForNewWindow(self, pos):
        new_browser = self.sender()
        if not isinstance(new_browser, QWebEngineView):
            return
        menu = QMenu()
        back_action = menu.addAction("Back")
        forward_action = menu.addAction("Forward")
        refresh_action = menu.addAction("Refresh")
        credits_action = menu.addAction("Credits")

        back_action.triggered.connect(lambda: new_browser.back())
        forward_action.triggered.connect(lambda: new_browser.forward())
        refresh_action.triggered.connect(lambda: new_browser.reload())
        credits_action.triggered.connect(self.show_credits_popup)
        menu.exec(new_browser.mapToGlobal(pos))

    def onCustomContextMenuRequested(self, pos, browser):
        menu = QMenu()
        new_tab_action = menu.addAction("Open link in new tab")
        new_window_action = menu.addAction("Open link in new window")
        back_action = menu.addAction("Back")
        forward_action = menu.addAction("Forward")
        refresh_action = menu.addAction("Refresh")
        show_tabs_action = menu.addAction("Show/Hide Tabs")
        credits_action = menu.addAction("Credits")

        link_url = browser.page().contextMenuData().linkUrl()
        if not link_url.isEmpty():
            new_tab_action.triggered.connect(lambda: self.create_new_tab(link_url.toString()))
            new_window_action.triggered.connect(lambda: self.open_new_window(link_url.toString()))
        else:
            new_tab_action.setEnabled(False)
            new_window_action.setEnabled(False)

        back_action.triggered.connect(lambda: browser.back())
        forward_action.triggered.connect(lambda: browser.forward())
        refresh_action.triggered.connect(lambda: browser.reload())
        show_tabs_action.triggered.connect(self.toggle_titles)
        credits_action.triggered.connect(self.show_credits_popup)
        menu.exec_(browser.mapToGlobal(pos))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = NovaNav(initial_url="https://www.google.com")
    window.show()
    sys.exit(app.exec_())
