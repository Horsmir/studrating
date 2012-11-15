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


#ifndef STUDENT_H
#define STUDENT_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QMap>

class QDataStream;

class Student : public QObject
{
	Q_OBJECT
public:
	explicit Student(QObject *parent = 0);
	Student(const Student &other);
	virtual ~Student();
	
	void setName(const QString &name);
	void setStudentId(quint32 studentId);
	void addRating(quint32 disciplinId, QVector< qreal > rating);
	void replaceRatings(quint32 disciplinId, quint32 dataId, const QVector<qreal> &rating);
	void replaceRating(quint32 disciplinId, quint32 dataId, quint32 ratId, const qreal &rating);
	
	void addOneEmptyRating(quint32 disciplinId);
	
	QString getName() const;
	quint32 getStudentId() const;
	QVector<qreal> getRating(quint32 disciplinId, quint32 dataId) const;
	qreal getAllRating(quint32 disciplinId, const QVector<qreal> &maxValues) const;
	
	Student &operator=(const Student &other);
	friend QDataStream &operator<<(QDataStream &out, const Student &student);
	friend QDataStream &operator>>(QDataStream &in, Student &student);
private:
	quint32 studentId;
	QString name; // имя студента
	QMap<quint32, QList<QVector<qreal> > > ratingsList; // список значений рейтинга: key - индекс дисциплины, value - список значений по критериям на каждое занятие
};

#endif // STUDENT_H
