import sys
from PyQt5.QtCore import Qt, QUrl
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QLineEdit, QPushButton, QWidget, QTabWidget, QShortcut, QDialog
from PyQt5.QtWebKitWidgets import QWebView

class URLInputDialog(QDialog):
    def __init__(self):
        super().__init__()
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
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.tab_widget = QTabWidget()
        self.tab_widget.setTabsClosable(True)
        self.tab_widget.tabCloseRequested.connect(self.close_tab)
        self.tab_widget.setMinimumWidth(200)  # Set a minimum width for the tabs

        self.layout.addWidget(self.tab_widget)

        self.url_input_dialog = URLInputDialog()
        self.url_input_dialog.ok_button.clicked.connect(self.open_new_tab)

        self.shortcut_new_tab = QShortcut("Ctrl+T", self)
        self.shortcut_new_tab.activated.connect(self.show_url_input_dialog)

        self.shortcut_zoom_in = QShortcut(Qt.CTRL + Qt.Key_Plus, self)
        self.shortcut_zoom_in.activated.connect(self.zoom_in)

        self.shortcut_zoom_out = QShortcut(Qt.CTRL + Qt.Key_Minus, self)
        self.shortcut_zoom_out.activated.connect(self.zoom_out)

        self.shortcut_toggle_titles = QShortcut(Qt.CTRL + Qt.Key_V, self)  # Change shortcut to Ctrl+V
        self.shortcut_toggle_titles.activated.connect(self.toggle_titles)

        self.create_new_tab("https://www.google.com")

    def create_new_tab(self, url):
        if not url.startswith("http"):
            url = "http://" + url
        browser = QWebView()
        browser.load(QUrl(url))
        browser.titleChanged.connect(lambda title, browser=browser: self.set_tab_title(browser, title[:20]))  # Limit title to 20 characters
        self.tab_widget.addTab(browser, "")

    def show_url_input_dialog(self):
        self.url_input_dialog.show()

    def open_new_tab(self):
        url = self.url_input_dialog.url_entry.text()
        if url:
            self.create_new_tab(url)
        self.url_input_dialog.hide()

    def close_tab(self, index):
        self.tab_widget.removeTab(index)

    def set_tab_title(self, browser, title):
        index = self.tab_widget.indexOf(browser)
        self.tab_widget.setTabText(index, title)

    def zoom_in(self):
        current_browser = self.tab_widget.currentWidget()
        if current_browser:
            current_browser.setZoomFactor(current_browser.zoomFactor() + 0.1)

    def zoom_out(self):
        current_browser = self.tab_widget.currentWidget()
        if current_browser:
            current_browser.setZoomFactor(current_browser.zoomFactor() - 0.1)

    def toggle_titles(self):
        for i in range(self.tab_widget.count()):
            browser = self.tab_widget.widget(i)
            title = browser.title() if self.tab_widget.tabBar().isVisible() else self.tab_widget.tabText(i)
            self.set_tab_title(browser, title[:20])
        self.tab_widget.tabBar().setVisible(not self.tab_widget.tabBar().isVisible())

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = NovaNav()
    window.show()
    sys.exit(app.exec_())
