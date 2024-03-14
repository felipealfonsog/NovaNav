use gtk::prelude::*;
use gio::prelude::*;
use gdk::enums::key;
use web_view::*;

fn main() {
    // Inicializar GTK
    gtk::init().expect("Fallo al inicializar GTK.");

    // Crear la aplicación
    let app = gtk::Application::new(
        Some("com.example.web_browser"),
        gio::ApplicationFlags::empty(),
    )
    .expect("Fallo al inicializar la aplicación...");

    // Conectar al evento activate
    app.connect_activate(|app| {
        // Crear la ventana principal
        let window = gtk::ApplicationWindow::new(app);
        window.set_default_size(800, 600);

        // Crear el notebook para las pestañas
        let notebook = gtk::Notebook::new();
        window.add(&notebook);

        // Conectar señales para los atajos de teclado
        let window_clone = window.clone();
        window.connect_key_release_event(move |_, event| {
            if let Some(key) = event.get_keyval() {
                if key == key::t && event.get_state().contains(gdk::ModifierType::CONTROL_MASK) {
                    let tab_label = gtk::Label::new(Some("Nueva Pestaña"));
                    let webview = WebViewBuilder::new().title("Nueva Pestaña").build().unwrap();
                    let tab_content = gtk::Box::new(gtk::Orientation::Vertical, 0);
                    tab_content.pack_start(&webview.gtk_widget(), true, true, 0);
                    let close_button = gtk::Button::with_label("x");
                    let notebook_clone = notebook.clone();
                    close_button.connect_clicked(move |_| {
                        if let Some(page_num) = notebook_clone.page_num(&tab_content) {
                            notebook_clone.remove_page(Some(page_num));
                        }
                    });
                    tab_content.pack_end(&close_button, false, true, 0);
                    notebook.append_page(&tab_content, Some(&tab_label));
                    webview.navigate("https://www.google.com").unwrap();
                    window_clone.show_all();
                    return Inhibit(true);
                } else if key == key::minus && event.get_state().contains(gdk::ModifierType::CONTROL_MASK) {
                    if let Some(page_num) = notebook.get_current_page() {
                        notebook.remove_page(Some(page_num));
                        window_clone.show_all();
                    }
                    return Inhibit(true);
                }
            }
            Inhibit(false)
        });

        // Mostrar todo
        window.show_all();
    });

    // Ejecutar la aplicación
    app.run(&[]);
}
