/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Роман Браун <firdragon76@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <QtCore/QDataStream>
#include "criterion.h"

Criterion::Criterion(QObject *parent): QObject(parent), name(QString()), value(0.0)
{

}

Criterion::Criterion(const QString &name, const qreal &value, QObject *parent): QObject(parent), name(name), value(value)
{

}

Criterion::Criterion(const Criterion &other): QObject(other.parent())
{
	name = other.name;
	value = other.value;
}

Criterion::~Criterion()
{

}

QString Criterion::getName() const
{
	return name;
}

qreal Criterion::getValue() const
{
 return value;
}

Criterion &Criterion::operator=(const Criterion &other)
{
	setParent(other.parent());
	name = other.name;
	value = other.value;
	return *this;
}

void Criterion::setName(const QString &name)
{
	this->name = name;
}

void Criterion::setValue(const qreal &value)
{
	this->value = value;
}

//=====================================================================================================//

QDataStream &operator<<(QDataStream &out, const Criterion &cr)
{
	out << cr.name << cr.value;
	return out;
}

QDataStream &operator>>(QDataStream &in, Criterion &cr)
{
	in >> cr.name;
	in >> cr.value;
	return in;
}

#include "criterion.moc"