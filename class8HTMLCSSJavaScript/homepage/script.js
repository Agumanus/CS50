document.addEventListener('DOMContentLoaded', function (){
    
    let trumpet = document.querySelector('.heading-trumpet');
    trumpet.addEventListener('click', function () {
        window.open('./trumpet.html');
    });

    let bud = document.querySelector('.heading-bud');
    bud.addEventListener('click', function () {
        window.open('./bud.html');
    });

    let taiwan = document.querySelector('.heading-taiwan');
    taiwan.addEventListener('click', function () {
        window.open('./taiwan.html');
    });
});