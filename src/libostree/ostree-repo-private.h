/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2011,2013 Colin Walters <walters@verbum.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#pragma once

#include "ostree-repo.h"

G_BEGIN_DECLS

struct OstreeRepo {
  GObject parent;

  GFile *repodir;
  GFile *tmp_dir;
  int    tmp_dir_fd;
  GFile *pending_dir;
  GFile *local_heads_dir;
  GFile *remote_heads_dir;
  GFile *objects_dir;
  GFile *uncompressed_objects_dir;
  GFile *remote_cache_dir;
  GFile *config_file;

  GFile *transaction_lock_path;
  GMutex txn_stats_lock;
  guint txn_metadata_objects_total;
  guint txn_metadata_objects_written;
  guint txn_content_objects_total;
  guint txn_content_objects_written;
  guint64 txn_content_bytes_written;

  GMutex cache_lock;
  GPtrArray *cached_meta_indexes;
  GPtrArray *cached_content_indexes;

  gboolean inited;
  gboolean in_transaction;
  GHashTable *loose_object_devino_hash;
  GHashTable *updated_uncompressed_dirs;

  GKeyFile *config;
  OstreeRepoMode mode;
  gboolean enable_uncompressed_cache;

  OstreeRepo *parent_repo;
};

GFile *
_ostree_repo_get_uncompressed_object_cache_path (OstreeRepo       *self,
                                                 const char       *checksum);

GFile *
_ostree_repo_get_file_object_path (OstreeRepo   *self,
                                   const char   *checksum);

gboolean
_ostree_repo_find_object (OstreeRepo           *self,
                          OstreeObjectType      objtype,
                          const char           *checksum,
                          GFile               **out_stored_path,
                          GCancellable         *cancellable,
                          GError             **error);

gboolean
_ostree_repo_get_loose_object_dirs (OstreeRepo       *self,
                                    GPtrArray       **out_object_dirs,
                                    GCancellable     *cancellable,
                                    GError          **error);

GFile *
_ostree_repo_get_object_path (OstreeRepo   *self,
                              const char   *checksum,
                              OstreeObjectType type);

gboolean
_ostree_repo_write_directory_meta (OstreeRepo   *self,
                                   GFileInfo    *file_info,
                                   GVariant     *xattrs,
                                   guchar      **out_csum,
                                   GCancellable *cancellable,
                                   GError      **error);

G_END_DECLS

