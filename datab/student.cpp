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
#include "student.h"

Student::Student(QObject *parent) : QObject(parent)
{
	name = QString();
	studentId = 0;
}

Student::Student(const Student &other) : QObject(other.parent())
{
	name = other.name;
	studentId = other.studentId;
	ratingsList = other.ratingsList;
}

Student::~Student()
{
	
}

void Student::setName(const QString &name)
{
	this->name = name;
}

QString Student::getName() const
{
	return name;
}

void Student::addRating(quint32 disciplinId, QVector< qreal > rating)
{
	QList<QVector<qreal> > lv = ratingsList.value(disciplinId);
	if(lv.isEmpty())
		ratingsList.insert(disciplinId, lv);
	ratingsList[disciplinId].append(rating);
}

QVector< qreal > Student::getRating(quint32 disciplinId, quint32 dataId) const
{
	Q_ASSERT(dataId < ratingsList.value(disciplinId).count());
	return ratingsList.value(disciplinId).at(dataId);
}

qreal Student::getAllRating(quint32 disciplinId, const QVector< qreal >& maxValues) const
{
	Q_ASSERT(maxValues.count() == ratingsList.value(disciplinId).at(0).count());
	QVector<qreal> sr;
	qreal res = 0.0, f = 0.0;
	foreach(sr, ratingsList.value(disciplinId))
	{
		for(int i = 0; i < maxValues.count(); i++)
			res += sr.at(i) * maxValues.at(i);
	}
	return res;
}

void Student::replaceRatings(quint32 disciplinId, quint32 dataId, const QVector< qreal >& rating)
{
	Q_ASSERT(dataId < ratingsList.value(disciplinId).count());
	ratingsList[disciplinId].replace(dataId, rating);
}

void Student::replaceRating(quint32 disciplinId, quint32 dataId, quint32 ratId, const qreal &rating)
{
	Q_ASSERT(dataId < ratingsList.value(disciplinId).count());
	ratingsList[disciplinId][dataId].replace(ratId, rating);
}

Student &Student::operator=(const Student &other)
{
	setParent(other.parent());
	studentId = other.studentId;
	name = other.name;
	ratingsList = other.ratingsList;
	return *this;
}

quint32 Student::getStudentId() const
{
	return studentId;
}

void Student::setStudentId(quint32 studentId)
{
	this->studentId = studentId;
}

void Student::addOneEmptyRating(quint32 disciplinId)
{
	if(!ratingsList.value(disciplinId).isEmpty())
	{	
		for(int i = 0; i < ratingsList.value(disciplinId).count(); i++)
			ratingsList[disciplinId][i].append(0.0);
	}
}

//=========================================================================================================//

QDataStream &operator<<(QDataStream &out, const Student &student)
{
	out << student.studentId << student.name << student.ratingsList;
	return out;
}

QDataStream &operator>>(QDataStream &in, Student &student)
{
	in >> student.studentId;
	in >> student.name;
	in >> student.ratingsList;
	return in;
}

#include "student.moc"