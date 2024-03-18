
/*

NovaWebEnginePage *webEnginePage;
webEnginePage = new NovaWebEnginePage();
delete webEnginePage;

#include "novawebenginepage.h" // Incluir el archivo de encabezado donde se define NovaWebEnginePage

void someFunction() {
    NovaWebEnginePage *webEnginePage = new NovaWebEnginePage();

    // alguna operación con webEnginePage

    delete webEnginePage; // Libera la memoria asignada cuando ya no se necesite
}

*/

#ifndef NOVAWEBENGINEPAGE_H
#define NOVAWEBENGINEPAGE_H

#include <QtWebEngineWidgets/QWebEnginePage>

// Clase NovaWebEnginePage que subclasifica QWebEnginePage
class NovaWebEnginePage : public QWebEnginePage
{
protected:
    // Sobrescribe la función createWindow para hacerla pública
    virtual QWebEnginePage *createWindow(WebWindowType type) override
    {
        // Llama a la implementación base de la función createWindow
        return QWebEnginePage::createWindow(type);
    }
};

#endif // NOVAWEBENGINEPAGE_H
