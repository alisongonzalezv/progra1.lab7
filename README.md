# Lab 7 - Agenda de Contactos (Qt C++)

## Información de entrega
- **Curso:** Programación 1
- **Laboratorio:** 7 - Sistema de Gestión de Contactos

## Pruebas de funcionamiento
- **Ruta del archivo de persistencia:** `~/Lab7Datos/contactos.txt`
- **Comprobación de Edición:** Se seleccionó una fila de la tabla `QTableWidget`, se abrieron los diálogos emergentes `QInputDialog` para cambiar los datos del contacto y se reescribió el archivo mediante `QFile::Truncate`.
- **Comprobación de Eliminación:** Se seleccionó un contacto, se confirmó la acción mediante un `QMessageBox::question` y se removió la línea correspondiente del archivo de texto.
