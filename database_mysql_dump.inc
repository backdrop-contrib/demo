<?php
// $Id$

// Some older mysql client libs are missing this constant.
if (!defined('MYSQLI_BINARY_FLAG')) {
  define('MYSQLI_BINARY_FLAG', 128);
}

/**
 * Dump active database.
 */
function demo_dump_db($filename, $exclude = array()) {
  // Make sure we have permission to save our backup file.
  $directory = dirname($filename);
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    return FALSE;
  }

  if ($fp = fopen($filename, 'wb')) {
    $header = "-- Demo.module database dump (version " . DEMO_DUMP_VERSION . ")\n";
    $header .= "-- http://drupal.org/project/demo\n";
    $header .= "--\n";
    $header .= "-- Database: " . _demo_get_database() . "\n";
    $header .= "-- Date: " . format_date(REQUEST_TIME, 'large') . "\n\n";
    // Avoid auto value for zero values (required for user id 0).
    $header .= "SET SQL_MODE=\"NO_AUTO_VALUE_ON_ZERO\";\n";
    // Temporarily disable foreign key checks for the time of import.
    $header .= "SET FOREIGN_KEY_CHECKS = 0;\n";
    fwrite($fp, $header);

    foreach (demo_enum_tables() as $table) {
      // Always export structure to allow creating a new site
      // from a database dump
      fwrite($fp, _demo_dump_table_structure($table));

      if (!in_array($table, $exclude)) {
        fwrite($fp, _demo_dump_table_data($table));
      }
    }

    // Re-enable foreign key checks.
    fwrite($fp, "\nSET FOREIGN_KEY_CHECKS = 1;\n");

    fclose($fp);
    return TRUE;
  }

  return FALSE;
}

/**
 * Returns the name of the active database.
 */
function _demo_get_database() {
  $database = array_keys(db_query('SHOW TABLES')->fetchAssoc());
  $database = preg_replace('/^Tables_in_/i', '', $database[0]);
  return $database;
}

/**
 * Dump table structure.
 */
function _demo_dump_table_structure($table) {
  $output = "\n";
  $output .= "--\n";
  $output .= "-- Table structure for table '$table'\n";
  $output .= "--\n\n";

  $data = db_query("SHOW CREATE TABLE `$table`")->fetchAssoc();
  $output .= preg_replace(
    array('/^CREATE TABLE/', '/"/'),
    array('CREATE TABLE IF NOT EXISTS', '`'),
    $data['create table']
  ) . ";\n";

  return $output;
}

/**
 * Dump table data.
 *
 * This code has largely been stolen from the phpMyAdmin project.
 */
function _demo_dump_table_data($table) {
  $output = "\n";
  $output .= "--\n";
  $output .= "-- Dumping data for table '$table'\n";
  $output .= "--\n\n";

  // Dump table data
  $result = db_query("SELECT * FROM `$table`", array(), array('fetch' => PDO::FETCH_ASSOC));

  // Get table fields.
  if ($fields = _demo_get_fields($result)) {
    // Disable indices to speed up import.
    $output .= "/*!40000 ALTER TABLE $table DISABLE KEYS */;\n";

    // Escape backslashes, PHP code, special chars
    $search = array('\\', "'", "\x00", "\x0a", "\x0d", "\x1a");
    $replace = array('\\\\', "''", '\0', '\n', '\r', '\Z');

    $insert_cmd = "INSERT INTO `$table` VALUES\n";
    $insert_buffer = '';
    $current_row = 0;
    $query_size = 0;

    foreach ($result as $row) {
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
        else if ($fields[$field]->numeric && !$fields[$field]->timestamp && !$fields[$field]->blob) {
          $values[] = $value;
        }
        // A true BLOB
        // - mysqldump only generates hex data when the --hex-blob
        //   option is used, for fields having the binary attribute
        //   no hex is generated
        // - a TEXT field returns type blob but a real blob
        //   returns also the 'binary' flag
        else if ($fields[$field]->binary && $fields[$field]->blob) {
          // Empty blobs need to be different, but '0' is also empty :-(
          if (empty($value) && $value != '0') {
            $values[] = "''";
          }
          else {
            $values[] = '0x' . bin2hex($value);
          }
        }
        // Something else -> treat as a string
        else {
          $values[] = "'" . str_replace($search, $replace, $value) . "'";
        }
        $field++;
      }

      if ($current_row == 1) {
        $insert_buffer = $insert_cmd . '(' . implode(', ', $values) . ')';
      }
      else {
        $insert_buffer = '(' . implode(', ', $values) . ')';
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

    // Enable indices again.
    $output .= "/*!40000 ALTER TABLE $table ENABLE KEYS */;\n";
  }

  return $output;
}

/**
 * Return table fields and their properties.
 */
function _demo_get_fields($result) {
  $fields = array();

  switch (db_driver()) {
    case 'mysql':
      $i = 0;
      while ($meta = $result->getColumnMeta($i)) {
        settype($meta, 'object');
        // pdo_mysql does not add a native type for INT fields.
        if (isset($meta->native_type)) {
          // Enhance the field definition for mysql-extension compatibilty.
          $meta->numeric = (strtolower($meta->native_type) == 'short');
          $meta->blob = (strtolower($meta->native_type) == 'blob');
          // Add custom properties.
          $meta->timestamp = (strtolower($meta->native_type) == 'long');
        }
        else {
          $meta->numeric = $meta->blob = $meta->timestamp = FALSE;
        }
        $meta->binary = (array_search('not_null', $meta->flags));
        $fields[] = $meta;
        $i++;
      }
      break;
  }

  return $fields;
}

