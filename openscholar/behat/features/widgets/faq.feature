Feature:
  Testing faq widget.

  @api @widgets @javascript
  Scenario: Verify "Recent FAQs" widget
    Given I am logging in as "john"
      And I visit "john/os/widget/boxes/os_faq_sv_list/edit/cp-layout"
      And I press "Save"
      And I visit "john"
      And I click the big gear
      And I click "Layout"
      And I drag the "Recent FAQs" widget to the "sidebar-first" region
      And I visit "john"
     Then I should see "RECENT FAQS"