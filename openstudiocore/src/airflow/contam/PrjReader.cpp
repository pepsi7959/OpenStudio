/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "PrjReader.hpp"
#include <iostream>
#include <stdlib.h>

#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace contam {

Reader::Reader(QFile *file) : m_lineNumber(0)
{
  m_stream.setDevice(file);
}

Reader::Reader(QString *string, int starting) : m_lineNumber(starting)
{
  m_stream.setString(string);
}

Reader::~Reader()
{
}

float Reader::readFloat()
{
  bool ok;
  QString string = readQString();
  float value = string.toFloat(&ok);
  if(!ok) {
    QString mesg=QString("Floating point (float) conversion error at line %1 for \"%2\"")
      .arg(m_lineNumber).arg(string);
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

double Reader::readDouble()
{
  bool ok;
  QString string = readQString();
  double value = string.toDouble(&ok);
  if(!ok) {
    QString mesg=QString("Floating point (double) conversion error at line %1 for \"%2\"")
      .arg(m_lineNumber).arg(string);
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

QString Reader::readQString()
{
  while(1) {
    while(m_entries.size() == 0) {
      QString input = m_stream.readLine();
      LOG(Debug, "Line read: " << input.toStdString());
      if(input.isNull()) {
        QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
        LOG_AND_THROW(mesg.toStdString());
      }
      m_lineNumber++;
      while(input[0]=='!') {
        input = m_stream.readLine();
        LOG(Debug, "Line read: " << input.toStdString());
        if(input.isNull()) {
          QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
          LOG_AND_THROW(mesg.toStdString());
        }
        m_lineNumber++;
      }
      m_entries = input.split(" ",QString::SkipEmptyParts);
    }
    QString out = m_entries.takeFirst();
    if(out[0] == '!') {
      m_entries.clear();
    } else {
      LOG(Debug, "String return: " << out.toStdString());
      return out;
    }
  }
}

std::string Reader::readStdString()
{
  return readQString().toStdString();
}

std::string Reader::readString()
{
  return readQString().toStdString();
}

int Reader::readInt()
{
  bool ok;
  QString string = readQString();
  int value = string.toInt(&ok);
  if(!ok) {
    QString mesg=QString("Integer conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string);
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

unsigned int Reader::readUInt()
{
  bool ok;
  QString string = readQString();
  int value = string.toUInt(&ok);
  if(!ok) {
    QString mesg=QString("Unsigned integer conversion error at line %1 for \"%2\"").arg(m_lineNumber).arg(string);
    LOG_AND_THROW(mesg.toStdString());
  }
  return value;
}

std::string Reader::readLine()
{
  return readLineQString().toStdString();
}

QString Reader::readLineQString()
{
  /* Dump any other input */
  if(m_entries.size()) {
    m_entries.clear();
  }
  QString input = m_stream.readLine();
  LOG(Debug, "Line read: " << input.toStdString());
  if(input.isNull()) {
    QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
    LOG_AND_THROW(mesg.toStdString());
  }
  m_lineNumber++;
  while(input[0]=='!') {
    input = m_stream.readLine();
    LOG(Debug, "Line read: " << input.toStdString());
    if(input.isNull()) {
      QString mesg=QString("Failed to read input at line %1").arg(m_lineNumber);
      LOG_AND_THROW(mesg.toStdString());
    }
    m_lineNumber++;
  }
  return input;
}

void Reader::read999()
{
  QString input = readLineQString();
  if(!input.startsWith(QString("-999"))) {
    QString mesg=QString("Failed to read -999 at line %1").arg(m_lineNumber);
    LOG_AND_THROW(mesg.toStdString());
  }
}

void Reader::read999(std::string mesg)
{
  QString input = readLineQString();
  if(!input.startsWith(QString("-999"))) {
    QString errmesg = QString().fromStdString(mesg) + QString(" at line %1").arg(m_lineNumber);
    LOG_AND_THROW(errmesg.toStdString());
  }
}

void Reader::readEnd()
{
  QString input = readLineQString();
  if(!input.startsWith(QString("* end project file."))) {
    QString mesg = QString("Failed to read file end at line %1").arg(m_lineNumber);
    LOG_AND_THROW(mesg.toStdString());
  }
}

void Reader::skipSection()
{
  readSection();
}

std::string Reader::readSection()
{
  QString section;
  while(1) {
    QString input = m_stream.readLine();
    if(input.isNull()) {
      QString mesg = QString("Failed to read input at line %1").arg(m_lineNumber);
      LOG_AND_THROW(mesg.toStdString());
    }
    m_lineNumber++;
    section += input + '\n';
    if(input.startsWith(QString("-999"))) {
      break;
    }
  }
  return section.toStdString();
}

std::vector<int> Reader::readIntVector(bool terminated)
{
  int n = readInt();
  std::vector<int> vector;
  for(int i=0;i<n;i++) {
    vector.push_back(readInt());
  }
  if(terminated) {
    read999("Failed to find section termination");
  }
  return vector;
}

template <> int Reader::read<int>()
{
  return readInt();
}

template <> unsigned int Reader::read<unsigned int>()
{
  return readUInt();
}

template <> double Reader::read<double>()
{
  return readDouble();
}

template <> float Reader::read<float>()
{
  return readFloat();
}

template <> QString Reader::read<QString>()
{
  return readQString();
}

template <> std::string Reader::read<std::string>()
{
  return readStdString();
}

template <> double Reader::readNumber<double>()
{
  return readInt();
}

template <> float Reader::readNumber<float>()
{
  return readFloat();
}

template <> QString Reader::readNumber<QString>()
{
  bool ok;
  QString string = readQString();
  string.toDouble(&ok);
  if(!ok) {
    QString mesg = QString("Invalid number \"%2\" on line %1").arg(m_lineNumber).arg(string);
    LOG_AND_THROW(mesg.toStdString());
  }
  return string;
}

template <> std::string Reader::readNumber<std::string>()
{
  bool ok;
  QString string = readQString();
  string.toDouble(&ok);
  if(!ok) {
    QString mesg = QString("Invalid number \"%2\" on line %1").arg(m_lineNumber).arg(string);
    LOG_AND_THROW(mesg.toStdString());
  }
  return string.toStdString();
}

} // contam
} // openstudio
