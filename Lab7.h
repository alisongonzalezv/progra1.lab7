#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include "ArchivoContactos.h"

class Lab7 : public QMainWindow {
    Q_OBJECT

public:
    Lab7(QWidget *parent = nullptr);
    ~Lab7();

private slots:
    void guardarContacto();
    void cargarHistorial();
    void prepararEdicion();
    void eliminarSeleccionado();

private:
    void crearInterfaz();
    void crearMenu();
    void limpiarFormulario();

    ArchivoContactos archivoContactos;
    QVector<Contacto> contactosLocales;

    int indiceEdicion = -1; // -1 significa que estamos registrando uno nuevo

    QStackedWidget *stackedWidget;

    // Vista Registro
    QLabel *lblTituloFormulario;
    QLineEdit *txtNombre;
    QLineEdit *txtTelefono;
    QLineEdit *txtCorreo;
    QPushButton *btnGuardar;
    QPushButton *btnCancelarEdicion;

    // Vista Historial
    QTableWidget *tablaContactos;
    QPushButton *btnEditar;
    QPushButton *btnEliminar;
};