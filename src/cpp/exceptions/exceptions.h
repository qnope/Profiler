#pragma once

#include <QString>
#include <exception>

struct NotOpenedFileException : std::exception {
    NotOpenedFileException(QString path) : path{std::move(path)} {}
    QString path;
};

struct NotParsableFileException : std::exception {
    NotParsableFileException(QString error) : error{std::move(error)} {}
    QString error;
};
