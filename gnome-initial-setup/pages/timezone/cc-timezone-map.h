/*
 * Copyright (C) 2010 Intel, Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Author: Thomas Wood <thomas.wood@intel.com>
 *
 */


#ifndef _CC_TIMEZONE_MAP_H
#define _CC_TIMEZONE_MAP_H

#include <gtk/gtk.h>

#define GWEATHER_I_KNOW_THIS_IS_UNSTABLE
#include <libgweather/gweather.h>

G_BEGIN_DECLS

#define CC_TYPE_TIMEZONE_MAP cc_timezone_map_get_type()

#define CC_TIMEZONE_MAP(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CC_TYPE_TIMEZONE_MAP, CcTimezoneMap))

#define CC_TIMEZONE_MAP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CC_TYPE_TIMEZONE_MAP, CcTimezoneMapClass))

#define CC_IS_TIMEZONE_MAP(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CC_TYPE_TIMEZONE_MAP))

#define CC_IS_TIMEZONE_MAP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CC_TYPE_TIMEZONE_MAP))

#define CC_TIMEZONE_MAP_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CC_TYPE_TIMEZONE_MAP, CcTimezoneMapClass))

typedef struct _CcTimezoneMap CcTimezoneMap;
typedef struct _CcTimezoneMapClass CcTimezoneMapClass;
typedef struct _CcTimezoneMapPrivate CcTimezoneMapPrivate;

struct _CcTimezoneMap
{
  GtkWidget parent;

  CcTimezoneMapPrivate *priv;
};

struct _CcTimezoneMapClass
{
  GtkWidgetClass parent_class;
};

GType cc_timezone_map_get_type (void) G_GNUC_CONST;

CcTimezoneMap *cc_timezone_map_new (void);

void cc_timezone_map_set_location (CcTimezoneMap    *map,
                                   GWeatherLocation *location);

G_END_DECLS

#endif /* _CC_TIMEZONE_MAP_H */