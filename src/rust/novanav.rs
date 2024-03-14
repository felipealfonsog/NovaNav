extern crate qt_widgets;
extern crate qt_core;
extern crate qt_webengine;

use qt_widgets::QApplication;
use qt_widgets::{QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QTabWidget, QLabel, QDialog};
use qt_core::{QString, Qt, Slot};
use qt_webengine::WebEngineView;
use std::rc::Rc;
use std::cell::RefCell;

fn main() {
    QApplication::init(|_app| {
        let window = NovaNav::new();
        window.show();
        QApplication::exec()
    });
}

struct NovaNav {
    central_widget: QWidget,
    layout: QVBoxLayout,
    tab_widget: QTabWidget,
    url_input_dialog: Rc<RefCell<URLInputDialog>>,
}

impl NovaNav {
    fn new() -> Rc<Self> {
        let window = Rc::new(NovaNav {
            central_widget: QWidget::new_0a(),
            layout: QVBoxLayout::new_1a(&QWidget::new_0a()),
            tab_widget: QTabWidget::new_0a(),
            url_input_dialog: Rc::new(RefCell::new(URLInputDialog::new())),
        });
        
        window.central_widget.set_layout(&window.layout);
        window.layout.add_widget(window.tab_widget.static_upcast());
        
        let shortcut_new_tab = window.tab_widget.register_shortcut("Ctrl+T");
        let shortcut_zoom_in = window.tab_widget.register_shortcut("Ctrl++");
        let shortcut_zoom_out = window.tab_widget.register_shortcut("Ctrl+-");
        let shortcut_toggle_titles = window.tab_widget.register_shortcut("Ctrl+V");
        
        shortcut_new_tab.activated().connect(&window.slot_show_url_input_dialog());
        shortcut_zoom_in.activated().connect(&window.slot_zoom_in());
        shortcut_zoom_out.activated().connect(&window.slot_zoom_out());
        shortcut_toggle_titles.activated().connect(&window.slot_toggle_titles());
        
        window.create_new_tab("https://www.google.com");
        window.set_permissions_and_settings();
        window.add_navigation_buttons();
        window.central_widget.adjust_size();
        
        window
    }
    
    fn add_navigation_buttons(&self) {
        let navigation_layout = QHBoxLayout::new_0a();
        
        let plus_button = QPushButton::from_q_string(&QString::from_std_str("+"));
        plus_button.set_tool_tip(&QString::from_std_str("Open Google"));
        plus_button.set_maximum_width(20);
        plus_button.clicked().connect(&self.slot_open_google_tab());
        
        let backward_button = QPushButton::from_q_string(&QString::from_std_str("<"));
        backward_button.set_tool_tip(&QString::from_std_str("Back"));
        backward_button.set_maximum_width(20);
        backward_button.clicked().connect(&self.slot_go_back());
        
        let forward_button = QPushButton::from_q_string(&QString::from_std_str(">"));
        forward_button.set_tool_tip(&QString::from_std_str("Forward"));
        forward_button.set_maximum_width(20);
        forward_button.clicked().connect(&self.slot_go_forward());
        
        let credits_button = QPushButton::from_q_string(&QString::from_std_str("="));
        credits_button.set_tool_tip(&QString::from_std_str("Credits"));
        credits_button.set_maximum_width(20);
        credits_button.clicked().connect(&self.slot_show_credits_popup());
        
        let ctrl_v_button = QPushButton::from_q_string(&QString::from_std_str("-"));
        ctrl_v_button.set_tool_tip(&QString::from_std_str("Ctrl + V"));
        ctrl_v_button.set_maximum_width(20);
        ctrl_v_button.clicked().connect(&self.slot_toggle_titles());
        
        navigation_layout.add_widget(&plus_button);
        navigation_layout.add_widget(&backward_button);
        navigation_layout.add_widget(&forward_button);
        navigation_layout.add_widget(&credits_button);
        navigation_layout.add_widget(&ctrl_v_button);
        navigation_layout.add_stretch_1a(1);
        
        let navigation_container = QWidget::new_0a();
        navigation_container.set_layout(&navigation_layout);
        
        self.tab_widget.set_corner_widget(&navigation_container, Qt::AlignmentFlag::from_int(0x0004));
    }
    
    fn create_new_tab(&self, url: &str) {
        let mut url = url.to_string();
        if !url.starts_with("http") {
            url = format!("http://{}", url);
        }
        
        let browser = ExternalLinkHandler::new();
        browser.set_url(&QUrl::from_q_string(&QString::from_std_str(&url)));
        browser.page().profile().set_http_user_agent(&QString::from_std_str("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36"));
        
        let browser_clone = browser.clone();
        browser.title_changed().connect(move |title| {
            self.set_tab_title(&browser_clone, &title[0..20]);
        });
        
        browser.page().full_screen_requested().connect(move |request| {
            request.accept();
        });
        
        browser.page().set_zoom_factor(0.65);
        
        self.tab_widget.add_tab(browser.into_raw_ptr(), &QString::new());
    }
    
    fn set_permissions_and_settings(&self) {
        let settings = QtWebEngineSettings::global_settings();
        settings.set_attribute_2a(0x0002, true);
        settings.set_attribute_2a(0x0001, true);
        settings.set_attribute_2a(0x0003, true);
        settings.set_attribute_2a(0x0004, true);
        settings.set_attribute_2a(0x0006, true);
        settings.set_attribute_2a(0x0007, true);
        settings.set_attribute_2a(0x0008, true);
        settings.set_attribute_2a(0x000B, true);
        settings.set_attribute_2a(0x000C, true);
        settings.set_attribute_2a(0x000D, true);
        settings.set_attribute_2a(0x000E, true);
    }
    
    fn slot_show_url_input_dialog(&self) -> Slot<'static> {
        Slot::new(move || {
            self.url_input_dialog.borrow().borrow().show();
        })
    }
    
    fn slot_open_google_tab(&self) -> Slot<'static> {
        Slot::new(|| {
            self.create_new_tab("https://www.google.com");
        })
    }
    
    fn slot_show_credits_popup(&self) -> Slot<'static> {
        Slot::new(|| {
            let credits_popup = QDialog::new_0a();
            credits_popup.set_window_title(&QString::from_std_str("Credits"));
            credits_popup.set_minimum_width(400);
            
            let credits_layout = QVBoxLayout::new_1a(&credits_popup);
            let credits_label_text = QString::from_std_str("Credits:\nEngineer: Felipe Alfonso Gonzalez\nGitHub: github.com/felipealfonsog\n\nThis software is licensed under the BSD 3-Clause License.\nFor details, see: https://opensource.org/licenses/BSD-3-Clause\n\nUsage restrictions:\nFor any inquiries about usage restrictions, please contact:\nFelipe Alfonso Gonzalez\nEmail: f.alfonso@res-ear.ch");
            let credits_label = QLabel::from_q_string(&credits_label_text);
            credits_label.set_alignment(Qt::AlignmentFlag::from_int(0x0084));
            credits_layout.add_widget(&credits_label);
            
            credits_popup.exec();
        })
    }
    
    fn slot_go_back(&self) -> Slot<'static> {
        Slot::new(|| {
            let current_browser = self.tab_widget.current_widget().as_ref();
            if let Some(browser) = current_browser {
                if browser.history().can_go_back() {
                    browser.back();
                }
            }
        })
    }
    
    fn slot_go_forward(&self) -> Slot<'static> {
        Slot::new(|| {
            let current_browser = self.tab_widget.current_widget().as_ref();
            if let Some(browser) = current_browser {
                if browser.history().can_go_forward() {
                    browser.forward();
                }
            }
        })
    }
    
    fn slot_toggle_titles(&self) -> Slot<'static> {
        Slot::new(|| {
            for i in 0..self.tab_widget.count() {
                let browser = self.tab_widget.widget(i).as_ref();
                let title = if self.tab_widget.tab_bar().is_visible() {
                    browser.title()
                } else {
                    self.tab_widget.tab_text(i)
                };
                self.set_tab_title(browser, &title[0..20]);
            }
            self.tab_widget.tab_bar().set_visible(!self.tab_widget.tab_bar().is_visible());
        })
    }
    
    fn set_tab_title(&self, browser: &WebEngineView, title: &str) {
        let index = self.tab_widget.index_of(browser);
        self.tab_widget.set_tab_text(index, &QString::from_std_str(title));
    }
}

struct URLInputDialog {
    dialog: QDialog,
    url_line_edit: QLineEdit,
    ok_button: QPushButton,
    cancel_button: QPushButton,
}

impl URLInputDialog {
    fn new() -> Self {
        let dialog = QDialog::new_0a();
        dialog.set_window_title(&QString::from_std_str("Enter URL"));

        let url_line_edit = QLineEdit::new();
        let ok_button = QPushButton::from_q_string(&QString::from_std_str("OK"));
        let cancel_button = QPushButton::from_q_string(&QString::from_std_str("Cancel"));

        let layout = QVBoxLayout::new_0a();
        layout.add_widget(&url_line_edit);
        layout.add_widget(&ok_button);
        layout.add_widget(&cancel_button);

        dialog.set_layout(&layout);

        URLInputDialog {
            dialog,
            url_line_edit,
            ok_button,
            cancel_button,
        }
    }

    fn show(&self) {
        self.dialog.exec();
    }
}

struct ExternalLinkHandler {
    view: WebEngineView,
}

impl ExternalLinkHandler {
    fn new() -> Self {
        ExternalLinkHandler {
            view: WebEngineView::new(),
        }
    }
}

impl Clone for ExternalLinkHandler {
    fn clone(&self) -> Self {
        ExternalLinkHandler {
            view: self.view.clone(),
        }
    }
}

impl Into<WebEngineView> for ExternalLinkHandler {
    fn into(self) -> WebEngineView {
        self.view
    }
}

impl Into<*mut WebEngineView> for ExternalLinkHandler {
    fn into(self) -> *mut WebEngineView {
        self.view.into_raw_ptr()
    }
}
