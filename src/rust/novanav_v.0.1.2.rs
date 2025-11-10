use gtk::prelude::*;
use gtk::{Application, ApplicationWindow, AccelGroup};
use gio::prelude::*;

fn main() {
    // Initialize GTK application
    let app = Application::new(
        Some("com.example.web_browser"),
        gio::ApplicationFlags::empty(),
    );

    // Connect to the activate signal to create the window
    app.connect_activate(|app| {
        // Create the main window
        let window = ApplicationWindow::new(app);
        window.set_title("NovaNav");
        window.set_default_size(600, 400);

        // Create an accelerator group to handle keyboard shortcuts
        let accel_group = AccelGroup::new();
        window.add_accel_group(&accel_group);

        // Shortcut to open a new tab (Ctrl + T)
        accel_group.connect_accel_closure(Some(&gtk::keys::constants::T), false, move |_| {
            // Your handling logic here
            glib::signal::Inhibit(false)
        });

        // Shortcut to close the current tab (Ctrl + W)
        accel_group.connect_accel_closure(Some(&gtk::keys::constants::W), false, move |_| {
            // Your handling logic here
            glib::signal::Inhibit(false)
        });

        // Show the window
        window.show_all();
    });

    // Run the application
    app.run(&[]);
}
