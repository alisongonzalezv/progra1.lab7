#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QSaveFile>
#include <QByteArray>

QString ArchivoContactos::ruta() const {
    return QDir::currentPath() + "/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
                              const QString& telefono,
                              const QString& correo) const {
    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();

    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();

    return completo && vaciado;
}

QVector<Contacto> ArchivoContactos::cargarTodos() const {
    QVector<Contacto> lista;
    QFile archivo(ruta());

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return lista;

    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        if (linea.trimmed().isEmpty()) continue;

        QStringList partes = linea.split(';');
        if (partes.size() == 3) {
            lista.append({partes[0], partes[1], partes[2]});
        }
    }
    archivo.close();
    return lista;
}

bool ArchivoContactos::guardarTodos(const QVector<Contacto>& contactos) const {
    QSaveFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&archivo);
    for (const auto& c : contactos) {
        out << c.nombre << ";" << c.telefono << ";" << c.correo << "\n";
    }
    return archivo.commit();
}