exports.config = {
  seleniumAddress: 'http://localhost:4444/wd/hub',
  specs: [
    'openscholar/modules/frontend/**/test.js'
  ]
}