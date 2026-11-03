#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

QString ArchivoContactos::ruta() const {
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre, const QString& telefono, const QString& correo) const {
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&archivo);
    out << nombre << ";" << telefono << ";" << correo << "\n";
    archivo.close();
    return true;
}

QVector<Contacto> ArchivoContactos::obtenerTodos() const {
    QVector<Contacto> lista;
    QFile archivo(ruta());
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return lista;

    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty()) continue;

        QStringList partes = linea.split(';');
        if (partes.size() == 3) {
            Contacto c;
            c.nombre = partes[0];
            c.telefono = partes[1];
            c.correo = partes[2];
            lista.append(c);
        }
    }
    archivo.close();
    return lista;
}

bool ArchivoContactos::reescribirTodo(const QVector<Contacto>& contactos) const {
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QTextStream out(&archivo);
    for (const Contacto& c : contactos) {
        out << c.nombre << ";" << c.telefono << ";" << c.correo << "\n";
    }
    archivo.close();
    return true;
}