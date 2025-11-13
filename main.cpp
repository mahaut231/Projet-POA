#include "Login.h"
#include "Mainwindow1.h"
#include "Mainwindow2.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        // Récupérer la centrale de l'utilisateur (par exemple via un getter)
        QString centrale = loginDialog.getCentraleReference(); // il faudra ajouter cette méthode

        if (centrale == "1") {
            MainWindow1 w1;
            w1.show();
            return a.exec();
        } else if (centrale == "2") {
            MainWindow2 w2;
            w2.show();
            return a.exec();
        }
    }

    return 0; // login annulé ou échoué
}
