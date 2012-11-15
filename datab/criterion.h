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


#ifndef CRITERION_H
#define CRITERION_H

#include <QtCore/QObject>

class QDataStream;

class Criterion : public QObject
{
	Q_OBJECT
public:
	explicit Criterion(QObject *parent = 0);
	explicit Criterion(const QString &name, const qreal &value, QObject *parent = 0);
	Criterion(const Criterion &other);
	virtual ~Criterion();
	
	void setName(const QString &name);
	void setValue(const qreal &value);
	
	QString getName() const;
	qreal getValue() const;
	
	Criterion &operator=(const Criterion &other);
	friend QDataStream &operator<<(QDataStream &out, const Criterion &cr);
	friend QDataStream &operator>>(QDataStream &in, Criterion &cr);
	
private:
	QString name;
	qreal value;
};

#endif // CRITERION_H
