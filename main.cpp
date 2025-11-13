#include "Mainwindow.h"
#include "Login.h"
#include <QApplication>
#include "Turbine.h"
int main(int argc, char *argv[])
{


    Turbine t1(1, 5000, false, "2020-03-15", 8000, 2000);
    t1.afficherInfos();
    t1.activer();
    t1.afficherInfos();


    QApplication a(argc, argv);

    // Créer la fenêtre de login
    Login loginDialog;

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
