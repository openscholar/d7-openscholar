<?php
// $Id$

/**
 * @file
 * Interface implemented by Rbuild module.
 *
 * @see /profiles/openscholar/modules/os_features/os_software/modules/rbuild/rbuild.handler.inc
 */

/**
 * Interface for a package handler
 * @author rbrandon
 *
 */
interface os_software_handler {

  /**
   * Returns the packages found at the target url.
   */
  public function get_packages();

  /**
   * Returns the active package.
   */
  public function get_package();

  /**
   * Alters a project node before it is saved.
   *
   * @param object $node
   */
  public function alter_project(&$node);
}

