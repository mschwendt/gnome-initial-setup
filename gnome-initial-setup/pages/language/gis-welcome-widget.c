/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/*
 * Copyright (C) 2013 Red Hat
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by:
 *     Jasper St. Pierre <jstpierre@mecheye.net>
 */

#include "config.h"
#include "gis-welcome-widget.h"

#include <locale.h>
#include <glib/gi18n.h>

#include "cc-common-language.h"

struct _GisWelcomeWidgetPrivate
{
  GtkWidget *stack;

  guint timeout_id;
};
typedef struct _GisWelcomeWidgetPrivate GisWelcomeWidgetPrivate;

#define TIMEOUT 5

G_DEFINE_TYPE_WITH_PRIVATE (GisWelcomeWidget, gis_welcome_widget, GTK_TYPE_BIN);

static gboolean
advance_stack (gpointer user_data)
{
  GisWelcomeWidget *widget = user_data;
  GisWelcomeWidgetPrivate *priv = gis_welcome_widget_get_instance_private (widget);
  GList *children, *l;

  children = gtk_container_get_children (GTK_CONTAINER (priv->stack));
  if (children == NULL)
    goto out;

  for (l = children; l != NULL; l = l->next)
    {
      if (l->data == gtk_stack_get_visible_child (GTK_STACK (priv->stack)))
        break;
    }

  /* wrap around */
  if (l->next)
    l = l->next;
  else
    l = children;

  gtk_stack_set_visible_child (GTK_STACK (priv->stack), l->data);

  g_list_free (children);

 out:
  return G_SOURCE_CONTINUE;
}

static void
gis_welcome_widget_start (GisWelcomeWidget *widget)
{
  GisWelcomeWidgetPrivate *priv = gis_welcome_widget_get_instance_private (widget);

  if (priv->timeout_id > 0)
    return;

  priv->timeout_id = g_timeout_add_seconds (5, advance_stack, widget);
}

static void
gis_welcome_widget_stop (GisWelcomeWidget *widget)
{
  GisWelcomeWidgetPrivate *priv = gis_welcome_widget_get_instance_private (widget);

  if (priv->timeout_id == 0)
    return;

  g_source_remove (priv->timeout_id);
  priv->timeout_id = 0;
}

static void
gis_welcome_widget_map (GtkWidget *widget)
{
  GTK_WIDGET_CLASS (gis_welcome_widget_parent_class)->map (widget);
  gis_welcome_widget_start (GIS_WELCOME_WIDGET (widget));
}

static void
gis_welcome_widget_unmap (GtkWidget *widget)
{
  GTK_WIDGET_CLASS (gis_welcome_widget_parent_class)->unmap (widget);
  gis_welcome_widget_stop (GIS_WELCOME_WIDGET (widget));
}

static char *
welcome (const char *locale_id)
{
  char *current_locale_id;
  char *welcome;

  current_locale_id = g_strdup (setlocale (LC_MESSAGES, NULL));
  setlocale (LC_MESSAGES, locale_id);
  welcome = _("Welcome!");
  setlocale (LC_MESSAGES, current_locale_id);
  g_free (current_locale_id);

  return welcome;
}

static GtkWidget *
big_label (const char *text)
{
  GtkWidget *label;
  PangoAttrList *attrs;

  label = gtk_label_new (text);

  attrs = pango_attr_list_new ();
  pango_attr_list_insert (attrs, pango_attr_scale_new (1.6));
  pango_attr_list_insert (attrs, pango_attr_weight_new (PANGO_WEIGHT_BOLD));
  gtk_label_set_attributes (GTK_LABEL (label), attrs);
  pango_attr_list_unref (attrs);

  return label;
}

static void
fill_stack (GisWelcomeWidget *widget)
{
  GisWelcomeWidgetPrivate *priv = gis_welcome_widget_get_instance_private (widget);
  GHashTable *initial = cc_common_language_get_initial_languages ();
  GHashTableIter iter;
  gpointer key, value;
  GHashTable *added_translations;

  added_translations = g_hash_table_new (g_str_hash, g_str_equal);

  g_hash_table_iter_init (&iter, initial);
  while (g_hash_table_iter_next (&iter, &key, &value))
    {
      char *locale_id = key;
      char *text;
      GtkWidget *label;

      text = welcome (locale_id);
      if (g_hash_table_contains (added_translations, text))
        continue;

      label = big_label (text);
      gtk_container_add (GTK_CONTAINER (priv->stack), label);
      gtk_widget_show (label);

      g_hash_table_add (added_translations, text);
    }

  g_hash_table_destroy (added_translations);
}

static void
gis_welcome_widget_constructed (GObject *object)
{
  fill_stack (GIS_WELCOME_WIDGET (object));
}

static void
gis_welcome_widget_class_init (GisWelcomeWidgetClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/initial-setup/gis-welcome-widget.ui");

  gtk_widget_class_bind_template_child_private (widget_class, GisWelcomeWidget, stack);

  object_class->constructed = gis_welcome_widget_constructed;
  widget_class->map = gis_welcome_widget_map;
  widget_class->unmap = gis_welcome_widget_unmap;
}

static void
gis_welcome_widget_init (GisWelcomeWidget *widget)
{
  gtk_widget_init_template (GTK_WIDGET (widget));
}
