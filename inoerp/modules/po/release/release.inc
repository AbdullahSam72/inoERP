<?php

 $bpa_line_id_a = [];
 $bpa_release_number_a = [];
// pa($$class);
 if(in_array($$class->po_type, array('STANDARD','CONTRACT'))){
  $$class = new po_header();
  $po_line_object = array(new po_line());
  $po_detail_object = array(new po_detail());
  $access_deined = 1;
  $msg = "<br>You cant open PO document in this form. Use PO  form";
 }
 
// if (empty($_GET['po_header_id']) && empty($_GET['ref_po_header_id']) && empty($_GET['po_number'])) {
//  $readonly = 1;
//  $mode = 2;
//  $msg = '<br> Invalid data selection - No BPA found . <br> <a href="javascript:history.go(-1)">Go back to the previous page</a>';
//  $access_deined = 1;
// }
 if (!empty($$class->po_header_id) && empty($$class->ref_po_header_id)) {
  $$class->ref_po_header_id = $$class->po_header_id;
 }

 if ($$class->po_type != 'BLANKET_RELEASE') {
  $$class->po_header_id = null;
  $po_line_object = array(new po_line());
 }


 if (empty($_GET['po_header_id']) && !empty($_GET['ref_po_header_id'])) {
  if (!empty($_GET['release_number'])) {
   $po_rls_header = po_header::find_by_refPOHeaderId_releseNumber($_GET['ref_po_header_id'], $_GET['release_number']);
   foreach ($po_rls_header as $key => $val) {
    $$class->$key = $val;
   }
   $po_line_object = po_line::find_by_parent_id($$class->po_header_id);
  } else {
   $$class = $$class->findBy_id($_GET['ref_po_header_id']);
   if (!empty($$class->po_header_id)) {
    $$class->ref_po_header_id = $$class->po_header_id;
    $$class->po_header_id = null;
    $$class->header_amount = null;
   }
  }
 }

// $prl = new sys_profile_line();
 if (!empty($$class->po_header_id)) {
  foreach ($po_line_object as &$po_lines) {
   if (!empty($po_lines->item_id_m)) {
    $item = item::find_by_id($po_lines->item_id_m);
    $po_lines->item_number = $item->item_number;
   }
  }
 } else {
  $buyer = user::find_by_user_name($session->username);
  $$class->buyer = $buyer->first_name . ' ' . $buyer->last_name;
  if (empty($$class->po_status)) {
   $$class->po_status = 'ENTERED';
  }
 }

//asign values of fields on form but not in databse only if $$class->$primary_cloumn is not nul
 if (!empty($$class->ref_po_header_id)) {
  $supplier_details = supplier::find_by_id($$class->supplier_id);
  $$class->supplier_name = $supplier_details->supplier_name;
  $$class->supplier_number = !empty($supplier_details->supplier_number) ? $supplier_details->supplier_number : null;
  $bpa_lines = po_line::find_by_parent_id($$class->ref_po_header_id);
  if (($bpa_lines)) {
   foreach ($bpa_lines as $bpa_line) {
    $bpa_line_id_a[$bpa_line->po_line_id] = $bpa_line->line_number;
   }
  }
  $all_releases = $$class->findAll_releases();
  if (($all_releases)) {
   foreach ($all_releases as $release) {
    $bpa_release_number_a[$release->release_number] = $release->release_number;
   }
  }
 }

 if (!empty($$class->po_status) && ($$class->po_status == 'INPROCESS')) {
  $readonly = 1;
  $mode = 2;
 }

 if (empty($$class->ref_po_header_id)) {
  $readonly = 1;
  $mode = 2;
 }
 $$class->po_type = 'BLANKET_RELEASE';

 if (empty($$class->release_number)) {
  $$class->po_status = 'ENTERED';
 }
?>