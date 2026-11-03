#ifndef LAB7_H
#define LAB7_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QStackedWidget>
#include "ArchivoContactos.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Lab7; }
QT_END_NAMESPACE

class Lab7 : public QMainWindow {
    Q_OBJECT

public:
    Lab7(QWidget *parent = nullptr);
    ~Lab7();

private slots:
    void guardarContacto();
    void cargarHistorial();
    void editarContacto();
    void eliminarContacto();
    void mostrarRegistrar();
    void mostrarHistorial();

private:
    Ui::Lab7 *ui;
    ArchivoContactos archivoContactos;

    // Componentes creados por código para evitar errores de UIC
    QStackedWidget *stackedWidget;
    QLineEdit *txtNombre;
    QLineEdit *txtTelefono;
    QLineEdit *txtCorreo;
    QPushButton *btnGuardar;
    QTableWidget *tablaContactos;
    QPushButton *btnEditar;
    QPushButton *btnEliminar;
};
#endif // LAB7_H