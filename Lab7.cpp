#include "Lab7.h"
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>

Lab7::Lab7(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Lab 7 — Contactos");
    resize(650, 480);

    crearInterfaz();
    crearMenu();
}

Lab7::~Lab7() {}

void Lab7::crearMenu() {
    QToolBar *navBar = addToolBar("Navegación");
    navBar->setMovable(false);
    navBar->setFloatable(false);

    QLabel *lblNav = new QLabel(" Navegación  ");
    QFont fontBold = lblNav->font();
    fontBold.setBold(true);
    lblNav->setFont(fontBold);
    navBar->addWidget(lblNav);

    QAction *actRegistrar = navBar->addAction("Registrar");
    QAction *actHistorial = navBar->addAction("Historial");

    connect(actRegistrar, &QAction::triggered, this, [this]() {
        limpiarFormulario();
        stackedWidget->setCurrentIndex(0);
    });

    connect(actHistorial, &QAction::triggered, this, [this]() {
        cargarHistorial();
        stackedWidget->setCurrentIndex(1);
    });
}

void Lab7::crearInterfaz() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    stackedWidget = new QStackedWidget(this);


    // PÁGINA 0: Formulario de Registro / Edición

    QWidget *pageRegistro = new QWidget();
    QVBoxLayout *layoutReg = new QVBoxLayout(pageRegistro);

    lblTituloFormulario = new QLabel("Registro de contactos");
    QFont fontTitulo = lblTituloFormulario->font();
    fontTitulo.setPointSize(16);
    fontTitulo.setBold(true);
    lblTituloFormulario->setFont(fontTitulo);

    txtNombre = new QLineEdit();
    txtTelefono = new QLineEdit();
    txtCorreo = new QLineEdit();

    btnGuardar = new QPushButton("Guardar");
    btnCancelarEdicion = new QPushButton("Cancelar");
    btnCancelarEdicion->setVisible(false); // Oculto por defecto

    QHBoxLayout *layoutBotonesForm = new QHBoxLayout();
    layoutBotonesForm->addWidget(btnGuardar);
    layoutBotonesForm->addWidget(btnCancelarEdicion);

    layoutReg->addWidget(lblTituloFormulario);
    layoutReg->addSpacing(10);
    layoutReg->addWidget(new QLabel("Nombre"));
    layoutReg->addWidget(txtNombre);
    layoutReg->addWidget(new QLabel("Teléfono"));
    layoutReg->addWidget(txtTelefono);
    layoutReg->addWidget(new QLabel("Correo"));
    layoutReg->addWidget(txtCorreo);
    layoutReg->addSpacing(15);
    layoutReg->addLayout(layoutBotonesForm);
    layoutReg->addStretch();


    // PÁGINA 1: Historial de Contactos

    QWidget *pageTabla = new QWidget();
    QVBoxLayout *layoutTabla = new QVBoxLayout(pageTabla);

    QLabel *lblTituloHistorial = new QLabel("Historial de contactos");
    lblTituloHistorial->setFont(fontTitulo);

    tablaContactos = new QTableWidget();
    tablaContactos->setColumnCount(3);
    tablaContactos->setHorizontalHeaderLabels({"Nombre", "Teléfono", "Correo"});
    tablaContactos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaContactos->setSelectionMode(QAbstractItemView::SingleSelection);
    tablaContactos->setEditTriggers(QAbstractItemView::NoEditTriggers); // Modificación en formulario únicamente
    tablaContactos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QHBoxLayout *layoutBotones = new QHBoxLayout();
    btnEditar = new QPushButton("Editar seleccionado");
    btnEliminar = new QPushButton("Eliminar seleccionado");

    layoutBotones->addWidget(btnEditar);
    layoutBotones->addWidget(btnEliminar);
    layoutBotones->addStretch();

    layoutTabla->addWidget(lblTituloHistorial);
    layoutTabla->addSpacing(10);
    layoutTabla->addWidget(tablaContactos);
    layoutTabla->addSpacing(10);
    layoutTabla->addLayout(layoutBotones);

    stackedWidget->addWidget(pageRegistro);
    stackedWidget->addWidget(pageTabla);
    mainLayout->addWidget(stackedWidget);

    // Conexiones de los botones
    connect(btnGuardar, &QPushButton::clicked, this, &Lab7::guardarContacto);
    connect(btnCancelarEdicion, &QPushButton::clicked, this, [this]() {
        limpiarFormulario();
        stackedWidget->setCurrentIndex(1);
    });
    connect(btnEditar, &QPushButton::clicked, this, &Lab7::prepararEdicion);
    connect(btnEliminar, &QPushButton::clicked, this, &Lab7::eliminarSeleccionado);
}

void Lab7::limpiarFormulario() {
    indiceEdicion = -1;
    lblTituloFormulario->setText("Registro de contactos");
    btnGuardar->setText("Guardar");
    btnCancelarEdicion->setVisible(false);
    txtNombre->clear();
    txtTelefono->clear();
    txtCorreo->clear();
}

void Lab7::guardarContacto() {
    QString nombre = txtNombre->text().trimmed();
    QString telefono = txtTelefono->text().trimmed();
    QString correo = txtCorreo->text().trimmed();

    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Completá los tres campos.");
        return;
    }

    if (nombre.contains(';') || telefono.contains(';') || correo.contains(';')) {
        QMessageBox::warning(this, "Caracter no permitido", "No usés el símbolo de punto y coma (;).");
        return;
    }

    // Modo Edición
    if (indiceEdicion >= 0) {
        contactosLocales[indiceEdicion] = {nombre, telefono, correo};
        if (archivoContactos.guardarTodos(contactosLocales)) {
            QMessageBox::information(this, "Éxito", "El contacto se actualizó correctamente.");
            limpiarFormulario();
            cargarHistorial();
            stackedWidget->setCurrentIndex(1); // Regresa al historial
        } else {
            QMessageBox::critical(this, "Error", "No se pudo actualizar el archivo.");
        }
    }
    // Modo Nuevo Registro
    else {
        if (!archivoContactos.guardar(nombre, telefono, correo)) {
            QMessageBox::critical(this, "Error", "No se pudo guardar el contacto.");
            return;
        }

        limpiarFormulario();
        QMessageBox::information(this, "Éxito", "El contacto se guardó correctamente.\nArchivo: " + archivoContactos.ruta());
        txtNombre->setFocus();
    }
}

void Lab7::cargarHistorial() {
    contactosLocales = archivoContactos.cargarTodos();
    tablaContactos->setRowCount(0);

    for (int i = 0; i < contactosLocales.size(); ++i) {
        tablaContactos->insertRow(i);
        tablaContactos->setItem(i, 0, new QTableWidgetItem(contactosLocales[i].nombre));
        tablaContactos->setItem(i, 1, new QTableWidgetItem(contactosLocales[i].telefono));
        tablaContactos->setItem(i, 2, new QTableWidgetItem(contactosLocales[i].correo));
    }
}

void Lab7::prepararEdicion() {
    int fila = tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Atención", "Seleccioná un contacto de la lista para editar.");
        return;
    }

    // Carga los datos seleccionados al formulario
    indiceEdicion = fila;
    txtNombre->setText(contactosLocales[fila].nombre);
    txtTelefono->setText(contactosLocales[fila].telefono);
    txtCorreo->setText(contactosLocales[fila].correo);

    // Cambia la interfaz para modo edición
    lblTituloFormulario->setText("Editar contacto");
    btnGuardar->setText("Actualizar");
    btnCancelarEdicion->setVisible(true);

    // Cambia automáticamente a la pantalla del formulario
    stackedWidget->setCurrentIndex(0);
}

void Lab7::eliminarSeleccionado() {
    int fila = tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Atención", "Seleccioná un contacto para eliminar.");
        return;
    }

    auto resp = QMessageBox::question(this, "Confirmar eliminación",
                                      "¿Estás seguro de que deseas eliminar este contacto?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (resp == QMessageBox::Yes) {
        contactosLocales.removeAt(fila);
        if (archivoContactos.guardarTodos(contactosLocales)) {
            QMessageBox::information(this, "Éxito", "El contacto ha sido eliminado.");
            cargarHistorial();
        } else {
            QMessageBox::critical(this, "Error", "No se pudo actualizar el archivo.");
        }
    }
}