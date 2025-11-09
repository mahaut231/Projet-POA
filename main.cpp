#include "mainwindow.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Créer la fenêtre de login
    login loginDialog;

    // Afficher le login en modal (bloque jusqu'à ce qu'il se ferme)
    if (loginDialog.exec() == QDialog::Accepted) // on peut utiliser Accepted pour futur gestion mot de passe
    {
        // Si login validé, créer la MainWindow
        MainWindow w;
        w.show();

        return a.exec(); // l'appli reste ouverte tant que MainWindow est ouverte
    }

    // Si login annulé ou fermé, quitter l'application
    return 0;
}
