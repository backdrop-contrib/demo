Demo
====

The Demo module has two main uses. It can be used to create and restore
snapshots of a site with a simple click that can be useful when testing
modules and during development. Also, with the included optional Demo
Reset module it can be used to setup a public demonstration site, a
sandbox for visitors to use, that is automatically reset periodically using
cron.

Installation
------------

- Install this module using the official Backdrop CMS instructions at https://backdropcms.org/guide/modules.

- Visit the configuration page under Administration > Structure > Snapshots (admin/structure/demo) and enter the required information.

Documentation
-------------

-- CONFIGURATION --

* Configure the path where dumps will be stored at the Dump settings
  (admin/structure/demo).

To configure automatic reset:

* Go to Manage snapshots (admin/structure/demo/manage) and select a snapshot
  for cron.

* Enable atomatic reset from Dump settings (admin/structure/demo). Make sure you
  have cron configured to run at least once within the entered time interval.


-- USAGE --

* Go to Create snapshot (admin/structure/demo/dump) and create your first
  snapshot.

* After a while, reset your site (admin/structure/demo/reset).

Issues
------

Bugs and Feature requests should be reported in the Issue Queue: https://github.com/backdrop-contrib/demo/issues.

Current Maintainers
-------------------

- Robert Garrigós (https://github.com/robertgarrigos).

Credits
-------

- Ported to Backdrop CMS by Robert Garrigós (https://github.com/robertgarrigos).
- Originally written for Drupal by Stefan M. Kudwien (smk-ka) - dev@unleashedmind.com and others.

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
