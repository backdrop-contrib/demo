<?php
// $Id$

/**
 * Dump active database.
 */
function demo_dump_db($filename, $exclude = array()) {
  // Make sure we have permission to save our backup file.
  if (!file_check_directory(dirname($filename), FILE_CREATE_DIRECTORY)) {
    return FALSE;
  }

  if ($fp = fopen($filename, 'wb')) {
    $header  = "-- Demo.module database dump\n";
    $header .= "-- http://drupal.org/project/demo\n";
    $header .= "--\n";
    $header .= "-- Database: ". _demo_get_database() ."\n";
    $header .= "-- Date: ". format_date(time(), 'large') ."\n\n";
    fwrite($fp, $header);

    foreach (demo_enum_tables() as $table) {
      if (!in_array($table, $exclude)) {
        fwrite($fp, _demo_dump_table($table));
      }
    }

    fclose($fp);
    return TRUE;
  }

  return FALSE;
}

/**
 * Returns the name of the active database.
 */
function _demo_get_database() {
  $database = array_keys(db_fetch_array(db_query('SHOW TABLES')));
  $database = preg_replace('/^Tables_in_/', '', $database[0]);
  return $database;
}

/**
 * Dump a table.
 *
 * This code has been stolen from the phpMyAdmin project.
 *
 * @todo Support extended inserts (much faster).
 */
function _demo_dump_table($table) {
  $output = "--\n";
  $output .= "-- Table structure for table '$table'\n";
  $output .= "--\n\n";

  $output .= _demo_show_create_table($table) .";\n\n";

  $output .= "--\n";
  $output .= "-- Dumping data for table '$table'\n";
  $output .= "--\n\n";
  
  // Dump table data
  $result = db_query("SELECT * FROM %s", $table);

  if (db_num_rows($result)) {
    // Get table fields.
    $fields = _demo_get_fields($result);
    $num_fields = count($fields);

    // Escape backslashes, PHP code, special chars
    $search = array('\\', "'", "\x00", "\x0a", "\x0d", "\x1a");
    $replace = array('\\\\', "''", '\0', '\n', '\r', '\Z');

    $insert_cmd = "INSERT INTO `$table` VALUES\n";
    $insert_buffer = '';
    $current_row = 0;
    $query_size = 0;

    while ($row = db_fetch_array($result)) {
      $current_row++;
      $values = array();
      $field = 0;
      foreach ($row as $value) {
        // NULL
        if (!isset($value) || is_null($value)) {
          $values[] = 'NULL';
        }
        // A number
        // timestamp is numeric on some MySQL 4.1, BLOBs are sometimes numeric
        else if ($fields['meta'][$field]->numeric
          && $fields['meta'][$field]->type != 'timestamp'
          && !($fields['meta'][$field]->blob)) {
          $values[] = $value;
        }
        // A binary field
        // Note: with mysqli, under MySQL 4.1.3, we get the flag
        // "binary" for those field types (I don't know why)
        else if (stristr($fields['flags'][$field], 'binary')
          && $fields['meta'][$field]->type != 'datetime'
          && $fields['meta'][$field]->type != 'date'
          && $fields['meta'][$field]->type != 'time'
          && $fields['meta'][$field]->type != 'timestamp') {
          // Empty blobs need to be different, but '0' is also empty :-(
          if (empty($value) && $value != '0') {
              $values[] = "''";
          } else {
              $values[] = '0x' . bin2hex($value);
          }
        }
        // Something else -> treat as a string
        else {
          $values[] = "'". str_replace($search, $replace, $value) ."'";
        }
        $field++;
      }

      if ($current_row == 1) {
        $insert_buffer = $insert_cmd . '('. implode(', ', $values) . ')';
      }
      else {
        $insert_buffer = '('. implode(', ', $values) . ')';
        if ($query_size + strlen($insert_buffer) > 50000) {
          $output .= ";\n";
          $current_row = 1;
          $query_size = 0;
          $insert_buffer = $insert_cmd . $insert_buffer;
        }
      }
      $query_size += strlen($insert_buffer);

      $output .= ($current_row == 1 ? '' : ",\n") . $insert_buffer;
    }

    if ($current_row > 0) {
      $output .= ";\n";
    }
  }

  return $output;
}

/**
 * Return CREATE TABLE definition.
 */
function _demo_show_create_table($table) {
  $create = db_fetch_array(db_query("SHOW CREATE TABLE %s", $table));
  return $create['Create Table'];
}

/**
 * Return table fields and their properties.
 */
function _demo_get_fields($result) {
  $fields = array();
  $num_fields = mysql_num_fields($result);
  for ($i = 0; $i < $num_fields; $i++) {
    $fields['meta'][] = mysql_fetch_field($result, $i);
    $fields['flags'][] = mysql_field_flags($result, $i);
  }
  return $fields;
}

