import sys
from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QTabWidget,
    QLineEdit,
    QPushButton,
    QDialog,
    QHBoxLayout,
    QLabel,
    QShortcut,
    QMenu,
    QAction,
    QTabBar,  # Agregado QTabBar aquí
)
from PyQt5.QtCore import Qt, QUrl
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEngineSettings, QWebEngineProfile


class URLInputDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Enter URL")
        layout = QVBoxLayout(self)
        self.url_entry = QLineEdit()
        layout.addWidget(self.url_entry)
        self.ok_button = QPushButton("OK")
        layout.addWidget(self.ok_button)
        self.ok_button.clicked.connect(self.accept)


class NovaNav(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NovaNav - Super Lightweight Browser")
        self.setGeometry(100, 100, 800, 900)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.tab_widget = QTabWidget()
        self.tab_widget.setTabsClosable(True)
        self.layout.addWidget(self.tab_widget)

        self.url_input_dialog = URLInputDialog(self)

        self.shortcut_new_tab = QShortcut(Qt.CTRL + Qt.Key_T, self)
        self.shortcut_new_tab.activated.connect(self.show_url_input_dialog)

        self.shortcut_zoom_in = QShortcut(Qt.CTRL + Qt.Key_Plus, self)
        self.shortcut_zoom_in.activated.connect(self.zoom_in)

        self.shortcut_zoom_out = QShortcut(Qt.CTRL + Qt.Key_Minus, self)
        self.shortcut_zoom_out.activated.connect(self.zoom_out)

        self.shortcut_toggle_titles = QShortcut(Qt.CTRL + Qt.Key_V, self)
        self.shortcut_toggle_titles.activated.connect(self.toggle_titles)

        self.create_new_tab("https://www.google.com")

        self.add_navigation_buttons()

        self.tab_widget.tabCloseRequested.connect(self.close_tab)

        shortcut_quit = QShortcut(Qt.CTRL + Qt.Key_Q, self)
        shortcut_quit.activated.connect(QApplication.quit)

  

    def show_url_input_dialog(self):
        if self.url_input_dialog.exec_() == QDialog.Accepted:
            url = self.url_input_dialog.url_entry.text()
            if not url.startswith("http://") and not url.startswith("https://"):
                url = "http://" + url
            self.create_new_tab(url)

    def close_tab(self, index):
        widget_to_remove = self.tab_widget.widget(index)
        browser_to_remove = widget_to_remove.findChild(QWebEngineView)

        if browser_to_remove:
            browser_to_remove.page().disconnect()
            browser_to_remove.deleteLater()

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
            title = browser.title() if isinstance(browser, QWebEngineView) else ""
            if not self.tab_widget.tabBar().isVisible():
                title = self.tab_widget.tabText(i)
            self.tab_widget.setTabText(i, title[:20])
        self.tab_widget.tabBar().setVisible(not self.tab_widget.tabBar().isVisible())

    def create_new_tab(self, url):
        browser = QWebEngineView()
        browser.setUrl(QUrl(url))
        browser.page().settings().setAttribute(QWebEngineSettings.JavascriptEnabled, True)
        browser.titleChanged.connect(lambda title: self.set_tab_title(browser, title[:20]))
        self.tab_widget.addTab(browser, "")
        
        # Verificar si hay un botón asociado antes de intentar ocultarlo
        if self.tab_widget.tabBar().tabButton(self.tab_widget.count() - 1, QTabBar.RightSide) is not None:
            self.tab_widget.tabBar().tabButton(self.tab_widget.count() - 1, QTabBar.RightSide).hide()


    def add_navigation_buttons(self):
        navigation_layout = QHBoxLayout()

        plus_button = QPushButton("+")
        plus_button.setFixedSize(10, 10)
        plus_button.clicked.connect(lambda: self.create_new_tab("https://www.google.com"))
        navigation_layout.addWidget(plus_button)

        backward_button = QPushButton("<")
        backward_button.setFixedSize(10, 10)
        backward_button.clicked.connect(self.go_back)
        navigation_layout.addWidget(backward_button)

        forward_button = QPushButton(">")
        forward_button.setFixedSize(10, 10)
        forward_button.clicked.connect(self.go_forward)
        navigation_layout.addWidget(forward_button)

        new_tab_button = QPushButton("o")
        new_tab_button.setFixedSize(10, 10)
        new_tab_button.clicked.connect(self.show_url_input_dialog)
        navigation_layout.addWidget(new_tab_button)

        credits_button = QPushButton("=")
        credits_button.setFixedSize(10, 10)
        credits_button.clicked.connect(self.show_credits_popup)
        navigation_layout.addWidget(credits_button)

        ctrl_v_button = QPushButton("-")
        ctrl_v_button.setFixedSize(10, 10)
        ctrl_v_button.clicked.connect(self.toggle_titles)
        navigation_layout.addWidget(ctrl_v_button)

        navigation_layout.addStretch()

        navigation_container = QWidget()
        navigation_container.setLayout(navigation_layout)

        self.tab_widget.setCornerWidget(navigation_container, Qt.TopRightCorner)





    def go_back(self):
        current_browser = self.tab_widget.currentWidget()
        if isinstance(current_browser, QWebEngineView):
            if current_browser.history().canGoBack():
                current_browser.back()

    def go_forward(self):
        current_browser = self.tab_widget.currentWidget()
        if isinstance(current_browser, QWebEngineView):
            if current_browser.history().canGoForward():
                current_browser.forward()

    def set_tab_title(self, browser, title):
        index = self.tab_widget.indexOf(browser)
        self.tab_widget.setTabText(index, title)

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




def main():
    app = QApplication(sys.argv)
    window = NovaNav()
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
