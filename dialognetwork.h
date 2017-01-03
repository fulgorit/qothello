#ifndef DIALOGNETWORK_H
#define DIALOGNETWORK_H

#include <QDialog>
#include <QAbstractSocket>
#include <QTcpSocket>

namespace Ui {
    class DialogNetwork;
}

class DialogNetwork : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNetwork(QWidget *parent = 0);
    ~DialogNetwork();
    QString colo;

private:
    Ui::DialogNetwork *ui;
    quint16 tailleMessage;
public:
    QTcpSocket *socket; // Représente le serveur


private slots:
    void on_boutonConnexion_clicked();
    void on_boutonEnvoyer_clicked();
    void on_message_returnPressed();
    void donneesRecues();
    void connecte();
    void deconnecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);

signals:
     void setBlack(QString,quint16);
     void setWhite(QString,quint16);

};

#endif // DIALOGNETWORK_H

