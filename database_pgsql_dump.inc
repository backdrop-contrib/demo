<?php
// $Id$

/**
 * Dump active database.
 *
 * @todo Postgres should utilize pg_dump. See phpPgAdmin's dbexport.php
 * for how to do it.
 */
function demo_dump_db($filename, $exclude = array()) {
  drupal_set_message(t('PostgreSQL support not implemented yet.'), 'error');
  return FALSE;
}

