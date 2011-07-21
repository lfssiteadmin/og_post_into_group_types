<?php
// $Id$

/**
 * @file
 * Admin settings for Organic Groups Post into Group Type module
 */

/**
 * Menu handler
 */
function og_post_group_types_settings() {
  $form = array();
  $options = array();
  $types = node_get_types();

  $og_types = og_get_types('group');

  foreach ($types as $_type) {
    $options[$_type->type] = t($_type->name);
  }

  // hide node types which are already serving as a group node
  foreach (og_get_types('group') as $val) {
    unset($options[$val]);
  }
  
  //!!need also to unset node types that are not set to either standard group post or wiki group post
/*
  $views = views_get_all_views();

  $views_options = array();
  foreach ($views as $key => $view) {
    if (substr($key, 0, strlen('og_ghp_')) == 'og_ghp_') {
      $views_options[$key] = $view->name;
    }
  }
*/
  $form['types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content type settings'),
    '#description' => t('For each group content type check those post types that can be posted in group of that type.'),
    '#tree' => TRUE,
  );

  foreach (node_get_types('types', NULL, TRUE) as $type) {
    $type_url_str = str_replace('_', '-', $type->type);
    if (in_array($type->type, $og_types)) {
      $form['types'][$type->type] = array(
        '#type' => 'fieldset',
        '#title' => $type->name,
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      foreach ($options as $type_key => $type_name) {
        if (variable_get('og_content_type_usage_'. $type_key, NULL) != 'omitted') {
          $canpost = variable_get('og_post_group_types_'. $type->type .'-'. $type_key, 0);
          $form['types'][$type->type][$type_key]['canpost'] = array(
            '#type' => 'checkbox',
            '#title' => t($type_name),
            '#default_value' => $canpost,
          );
    
        }
      }

    }

  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Submit handler for og_post_group_types_settings()
 */
function og_post_group_types_settings_validate($form, &$form_state) {
  //currently no validation
}

/**
 * Submit handler for og_post_group_types_settings()
 */
 
function og_post_group_types_settings_submit($form, &$form_state) {
  db_query('DELETE FROM {variable} WHERE name LIKE "og_post_group_types_%"');
  cache_clear_all('variables', 'cache');
  foreach ($form_state['values']['types'] as $type => $grouptype) {
    foreach ($grouptype as $key => $value) {
      if ($value['canpost'] == 1) {
        variable_set('og_post_group_types_'. $type .'-'. $key, 1);
      }
    }
  } 
  drupal_set_message('Your changes have been saved.');
}

