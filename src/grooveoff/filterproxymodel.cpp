/*
    GrooveOff - Offline Grooveshark.com music
    Copyright (C) 2013  Giuseppe Calà <jiveaxe@gmail.com>

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


#include "filterproxymodel.h"
#include "roles.h"

FilterProxyModel::FilterProxyModel(QObject* parent): QSortFilterProxyModel(parent)
{

}

bool FilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    return ((artist_ == "All Artists") ? true : (index.data(SongRoles::Artist).toString() == artist_)) &&
           ((album_  == "All Albums")  ? true : (index.data(SongRoles::Album).toString() == album_));


}

void FilterProxyModel::setMatchTerms(const QString& artist, const QString& album)
{
    artist_ = artist;
    album_ = album;
}


