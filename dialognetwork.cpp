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

    // On annonce sur la fen�tre qu'on est en train de se connecter
    ui->listeMessages->append(tr("<em>Tentative de connexion en cours...</em>"));
   // ui->boutonConnexion->setEnabled(false);

    socket->abort(); // On d�sactive les connexions pr�c�dentes s'il y en a
    socket->connectToHost(ui->serveurIP->text(), ui->serveurPort->value()); // On se connecte au serveur demand�

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On pr�pare le paquet � envoyer
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

    // On pr�pare le paquet � envoyer
    //QString messageAEnvoyer = tr("<strong>") + pseudo->text() +tr("</strong> : ") + message->text();

    QString messageAEnvoyer = "Bonjour";

    out << (quint16) 0;
    out << messageAEnvoyer;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet); // On envoie le paquet


}

// Appuyer sur la touche Entr�e a le m�me effet que cliquer sur le bouton "Envoyer"
void DialogNetwork::on_message_returnPressed()
{
    on_boutonEnvoyer_clicked();
}

// On a re�u un paquet (ou un sous-paquet)
void DialogNetwork::donneesRecues()
{
    /* M�me principe que lorsque le serveur re�oit un paquet :
    On essaie de r�cup�rer la taille du message
    Une fois qu'on l'a, on attend d'avoir re�u le message entier (en se basant sur la taille annonc�e tailleMessage)
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


    // Si on arrive jusqu'� cette ligne, on peut r�cup�rer le message entier
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

    // On remet la taille du message � 0 pour pouvoir recevoir de futurs messages
    tailleMessage = 0;
}

// Ce slot est appel� lorsque la connexion au serveur a r�ussi
void DialogNetwork::connecte()
{
    ui->listeMessages->append(tr("<em>Connexion r�ussie !</em>"));


}

// Ce slot est appel� lorsqu'on est d�connect� du serveur
void DialogNetwork::deconnecte()
{
    ui->listeMessages->append(tr("<em>D�connect� du serveur</em>"));
}

// Ce slot est appel� lorsqu'il y a une erreur
void DialogNetwork::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur) // On affiche un message diff�rent selon l'erreur qu'on nous indique
    {
        case QAbstractSocket::HostNotFoundError:
            ui->listeMessages->append(tr("<em>ERREUR : le serveur n'a pas pu �tre trouv�. V�rifiez l'IP et le port.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            ui->listeMessages->append(tr("<em>ERREUR : le serveur a refus� la connexion. V�rifiez si le programme \"serveur\" a bien �t� lanc�. V�rifiez aussi l'IP et le port.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            ui->listeMessages->append(tr("<em>ERREUR : le serveur a coup� la connexion.</em>"));
            break;
        default:
            ui->listeMessages->append(tr("<em>ERREUR : ") + socket->errorString() + tr("</em>"));
    }

    ui->boutonConnexion->setEnabled(true);
}
