#include "dialognetwork.h"
#include "ui_dialognetwork.h"
#include <QTcpSocket>
#include <QMessageBox>
#include "QMainWindow"
#include <QHostAddress>

DialogNetwork::DialogNetwork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNetwork)
{

    ui->setupUi(this);
    ui->serveurIP->setText("127.0.0.1");
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));

    tailleMessage = 0;

    ui->radioButton->setChecked(true);



}

DialogNetwork::~DialogNetwork()
{
    delete ui;
}



// Tentative de connexion au serveur
void DialogNetwork::on_boutonConnexion_clicked()
{
    if (ui->radioButton->isChecked()||ui->radioButton_2->isChecked()) {

    // On annonce sur la fenêtre qu'on est en train de se connecter
    ui->listeMessages->append(tr("<em>Tentative de connexion en cours...</em>"));
   // ui->boutonConnexion->setEnabled(false);

    socket->abort(); // On désactive les connexions précédentes s'il y en a
    socket->connectToHost(ui->serveurIP->text(), ui->serveurPort->value()); // On se connecte au serveur demandé

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer
    //QString messageAEnvoyer = tr("<strong>") + pseudo->text() +tr("</strong> : ") + message->text();

    QString messageAEnvoyer;

    if(ui->radioButton->isChecked()) messageAEnvoyer="BLACK";
    if(ui->radioButton_2->isChecked()) messageAEnvoyer="WHITE";


    out << (quint16) 0;
    out << messageAEnvoyer;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet); // On envoie le paquet

    } else {


        QMessageBox msgBox;
        msgBox.setText("Il faut choisir BLACK ou WHITE.");
        msgBox.exec();


    }
}

// Envoi d'un message au serveur
void DialogNetwork::on_boutonEnvoyer_clicked()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer
    //QString messageAEnvoyer = tr("<strong>") + pseudo->text() +tr("</strong> : ") + message->text();

    QString messageAEnvoyer = "Bonjour";

    out << (quint16) 0;
    out << messageAEnvoyer;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet); // On envoie le paquet


}

// Appuyer sur la touche Entrée a le même effet que cliquer sur le bouton "Envoyer"
void DialogNetwork::on_message_returnPressed()
{
    on_boutonEnvoyer_clicked();
}

// On a reçu un paquet (ou un sous-paquet)
void DialogNetwork::donneesRecues()
{
    /* Même principe que lorsque le serveur reçoit un paquet :
    On essaie de récupérer la taille du message
    Une fois qu'on l'a, on attend d'avoir reçu le message entier (en se basant sur la taille annoncée tailleMessage)
    */
    QDataStream in(socket);

    if (tailleMessage == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
             return;

        in >> tailleMessage;
    }

    if (socket->bytesAvailable() < tailleMessage)
        return;


    // Si on arrive jusqu'à cette ligne, on peut récupérer le message entier
    QString messageRecu;
    in >> messageRecu;

    bool dontprint=false;


    if (messageRecu.contains("ERR")) {

        dontprint=true;
        ui->listeMessages->append("Erreur: choissez l'autre couleur.");


    }

    if (messageRecu.contains("ACKGAMEBEGINW")) { emit setWhite(socket->peerAddress().toString(),socket->peerPort()); this->close();   }
    if (messageRecu.contains("ACKGAMEBEGINB")) { emit setBlack(socket->peerAddress().toString(),socket->peerPort()); this->close();   }

    if (messageRecu.contains("BLACK")) {

        ui->boutonConnexion->setEnabled(false);
        ui->radioButton->setEnabled(false);
        ui->radioButton_2->setVisible(false);
        dontprint=true;

    }

    if (messageRecu.contains("WHITE")) {

        ui->boutonConnexion->setEnabled(false);
        ui->radioButton->setVisible(false);
        ui->radioButton_2->setEnabled(false);
        dontprint=true;
    }

    // On affiche le message sur la zone de Chat
    if (!dontprint) ui->listeMessages->append(messageRecu);

    // On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
    tailleMessage = 0;
}

// Ce slot est appelé lorsque la connexion au serveur a réussi
void DialogNetwork::connecte()
{
    ui->listeMessages->append(tr("<em>Connexion réussie !</em>"));


}

// Ce slot est appelé lorsqu'on est déconnecté du serveur
void DialogNetwork::deconnecte()
{
    ui->listeMessages->append(tr("<em>Déconnecté du serveur</em>"));
}

// Ce slot est appelé lorsqu'il y a une erreur
void DialogNetwork::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message différent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            ui->listeMessages->append(tr("<em>ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            ui->listeMessages->append(tr("<em>ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            ui->listeMessages->append(tr("<em>ERREUR : le serveur a coupé la connexion.</em>"));
            break;
        default:
            ui->listeMessages->append(tr("<em>ERREUR : ") + socket->errorString() + tr("</em>"));
    }

    ui->boutonConnexion->setEnabled(true);
}
