Demo
====

Demo Site is a simple module that allows you to take a snapshot of a Drupal
demonstration site. It turns a Drupal installation into a sandbox that you can
use for testing modules or publicly demonstrating a module / extension / theme
(you name it). In short: With cron enabled, the Drupal site will be reset to
the dumped state in a definable interval. Of course you can reset the site
manually, too.

Installation
------------

- Install this module using the official Backdrop CMS instructions at https://backdropcms.org/guide/modules.

- Visit the configuration page under Administration > Structure > Snapshots (admin/structure/demo) and enter the required information.

- Any additional steps.

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
