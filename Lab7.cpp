#include "Lab7.h"
#include "ui_Lab7.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenuBar>
#include <QAction>
#include <QTableWidgetItem>
#include <QHeaderView>

Lab7::Lab7(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Lab7) {
    ui->setupUi(this);

    // Barra de menú superior (Navegación)
    QMenu *menuNav = menuBar()->addMenu("Navegación");
    QAction *actReg = menuNav->addAction("Registrar");
    QAction *actHist = menuNav->addAction("Historial / Editar / Eliminar");

    connect(actReg, &QAction::triggered, this, &Lab7::mostrarRegistrar);
    connect(actHist, &QAction::triggered, this, &Lab7::mostrarHistorial);

    stackedWidget = new QStackedWidget(this);

    // --- PÁGINA 1: REGISTRAR ---
    QWidget *pageReg = new QWidget(this);
    QVBoxLayout *layoutReg = new QVBoxLayout(pageReg);

    QLabel *lblTituloReg = new QLabel("<h2>Registro de contactos</h2>", pageReg);
    txtNombre = new QLineEdit(pageReg);
    txtTelefono = new QLineEdit(pageReg);
    txtCorreo = new QLineEdit(pageReg);
    btnGuardar = new QPushButton("Guardar", pageReg);

    QFormLayout *form = new QFormLayout();
    form->addRow("Nombre:", txtNombre);
    form->addRow("Teléfono:", txtTelefono);
    form->addRow("Correo:", txtCorreo);

    layoutReg->addWidget(lblTituloReg);
    layoutReg->addLayout(form);
    layoutReg->addWidget(btnGuardar);
    layoutReg->addStretch();

    // --- PÁGINA 2: HISTORIAL ---
    QWidget *pageHist = new QWidget(this);
    QVBoxLayout *layoutHist = new QVBoxLayout(pageHist);

    QLabel *lblTituloHist = new QLabel("<h2>Historial de contactos</h2>", pageHist);
    tablaContactos = new QTableWidget(0, 3, pageHist);
    tablaContactos->setHorizontalHeaderLabels({"Nombre", "Teléfono", "Correo"});
    tablaContactos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaContactos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaContactos->setSelectionMode(QAbstractItemView::SingleSelection);
    tablaContactos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnEditar = new QPushButton("Editar seleccionado", pageHist);
    btnEliminar = new QPushButton("Eliminar seleccionado", pageHist);
    btnLayout->addWidget(btnEditar);
    btnLayout->addWidget(btnEliminar);

    layoutHist->addWidget(lblTituloHist);
    layoutHist->addWidget(tablaContactos);
    layoutHist->addLayout(btnLayout);

    stackedWidget->addWidget(pageReg);
    stackedWidget->addWidget(pageHist);
    setCentralWidget(stackedWidget);

    connect(btnGuardar, &QPushButton::clicked, this, &Lab7::guardarContacto);
    connect(btnEditar, &QPushButton::clicked, this, &Lab7::editarContacto);
    connect(btnEliminar, &QPushButton::clicked, this, &Lab7::eliminarContacto);
}

Lab7::~Lab7() {
    delete ui;
}

void Lab7::mostrarRegistrar() {
    stackedWidget->setCurrentIndex(0);
}

void Lab7::mostrarHistorial() {
    stackedWidget->setCurrentIndex(1);
    cargarHistorial();
}

void Lab7::guardarContacto() {
    QString nombre = txtNombre->text().trimmed();
    QString telefono = txtTelefono->text().trimmed();
    QString correo = txtCorreo->text().trimmed();

    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Completá los tres campos.");
        return;
    }

    for (const QString& dato : QStringList{nombre, telefono, correo}) {
        if (dato.contains(';') || dato.contains('\n') || dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido", "No usés punto y coma ni saltos de línea.");
            return;
        }
    }

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
                              "Revisá permisos y espacio disponible.\nArchivo: " + archivoContactos.ruta());
        return;
    }

    txtNombre->clear();
    txtTelefono->clear();
    txtCorreo->clear();

    QMessageBox::information(this, "Guardado completo",
                             "El contacto se guardó correctamente.\nArchivo: " + archivoContactos.ruta());
    txtNombre->setFocus();
}

void Lab7::cargarHistorial() {
    tablaContactos->setRowCount(0);
    QVector<Contacto> contactos = archivoContactos.obtenerTodos();

    for (int i = 0; i < contactos.size(); ++i) {
        tablaContactos->insertRow(i);
        tablaContactos->setItem(i, 0, new QTableWidgetItem(contactos[i].nombre));
        tablaContactos->setItem(i, 1, new QTableWidgetItem(contactos[i].telefono));
        tablaContactos->setItem(i, 2, new QTableWidgetItem(contactos[i].correo));
    }
}

void Lab7::editarContacto() {
    int fila = tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Atención", "Seleccioná una fila para editar.");
        return;
    }

    QVector<Contacto> contactos = archivoContactos.obtenerTodos();
    Contacto &c = contactos[fila];

    bool ok1, ok2, ok3;
    QString nuevoNombre = QInputDialog::getText(this, "Editar Nombre", "Nombre:", QLineEdit::Normal, c.nombre, &ok1);
    if (!ok1 || nuevoNombre.trimmed().isEmpty()) return;

    QString nuevoTel = QInputDialog::getText(this, "Editar Teléfono", "Teléfono:", QLineEdit::Normal, c.telefono, &ok2);
    if (!ok2 || nuevoTel.trimmed().isEmpty()) return;

    QString nuevoCorreo = QInputDialog::getText(this, "Editar Correo", "Correo:", QLineEdit::Normal, c.correo, &ok3);
    if (!ok3 || nuevoCorreo.trimmed().isEmpty()) return;

    c.nombre = nuevoNombre.trimmed();
    c.telefono = nuevoTel.trimmed();
    c.correo = nuevoCorreo.trimmed();

    if (archivoContactos.reescribirTodo(contactos)) {
        QMessageBox::information(this, "Éxito", "Contacto actualizado.");
        cargarHistorial();
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el archivo.");
    }
}

void Lab7::eliminarContacto() {
    int fila = tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Atención", "Seleccioná una fila para eliminar.");
        return;
    }

    QMessageBox::StandardButton resp = QMessageBox::question(
        this, "Confirmar eliminación",
        "¿Estás seguro de que deseas eliminar este contacto?",
        QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        QVector<Contacto> contactos = archivoContactos.obtenerTodos();
        contactos.removeAt(fila);

        if (archivoContactos.reescribirTodo(contactos)) {
            QMessageBox::information(this, "Éxito", "Contacto eliminado.");
            cargarHistorial();
        } else {
            QMessageBox::critical(this, "Error", "No se pudo modificar el archivo.");
        }
    }
}