#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PerlinNoise.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }    
    this->setTouchEnabled(true);
    
    leftTouch = NULL;
    rightTouch = NULL;
    
    agitationIndex = 500;
    
    CCSize texSize = CCSizeMake(this->boundingBox().getMaxX()/6, this->boundingBox().getMaxY()/6);
    int bytes = texSize.width * texSize.height * 4;
    void* textureData = malloc(bytes);
    memset(textureData, INT32_MAX, bytes);
    
    texture = new CCTexture2DMutable();
    if (!texture->initWithData(textureData, kCCTexture2DPixelFormat_RGBA8888, texSize.width, texSize.height, texSize)) {
        CCLOG("Could not create texture");
        delete texture;
        texture = NULL;
    }
    
    texture->setAliasTexParameters();
    
    renderSprite = new CCSprite();
    renderSprite->initWithTexture(texture);
    renderSprite->setPosition(CCPoint(this->boundingBox().getMidX(), this->boundingBox().getMidY()));
    this->addChild(renderSprite);
    texture->retain();
    
    this->schedule(schedule_selector(HelloWorld::tick), .0001);
    
    return true;
}

void HelloWorld::tick(float dt){
    if(leftTouch != NULL && rightTouch != NULL){
        CCPoint leftTouchLocation = leftTouch->getLocationInView();
        leftTouchLocation = CCDirector::sharedDirector()->convertToGL(leftTouchLocation);
        CCPoint rightTouchLocation = rightTouch->getLocationInView();
        rightTouchLocation = CCDirector::sharedDirector()->convertToGL(rightTouchLocation);
        if(abs(rightTouchLocation.y - leftTouchLocation.y) < 50 && abs(rightTouchLocation.y-lineHeight) < 50){
            printf("%d: synched\n", frameCount);
        }
    }
    lineHeight = abs(sin(frameCount*.01)*(this->boundingBox().getMaxY()*PerlinNoise::noise(frameCount*.00001*agitationIndex, 10)));
    lineHeight = 0;
    frameCount++;
    
    texture->fill(ccc4(0, 0, 0, 0));
    static int iter = 0;
    for(int j = 0; j < 6; j++){
        for(int i = 0; i < texture->getContentSize().width; i++){
            ccColor4B color = ccc4(255, 255, 255, 255);
            texture->setPixelAt(CCPoint(i+.03*(texture->getContentSize().width-i)*PerlinNoise::noise((i+iter)*.03, 10*j),
                                        lineHeight+(j*(texture->getContentSize().height/6))+((texture->getContentSize().width-i)*.4)*
                                        PerlinNoise::noise((i*.7+iter)*.03, 10*j)),
                                color);
        }
    }
    iter++;
    texture->apply();
    
    renderSprite->initWithTexture(texture);
    renderSprite->setScale(6);
}

void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event){
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        //printf("%0.2f x %0.2f\n",touchLocation.x,touchLocation.y);
        
        if (leftTouch == NULL) {
            if (touchLocation.x < this->boundingBox().getMidX() &&
                this->boundingBox().getMinX() + 100 > touchLocation.x) {
                leftTouch = touch;
                //printf("got left touch\n");
            }
        }
        
        if(rightTouch == NULL){
            if(touchLocation.x > this->boundingBox().getMidX() &&
               this->boundingBox().getMaxX() - 100 < touchLocation.x){
                rightTouch = touch;
                //printf("got right touch\n");
            }
        }
    }
}

void HelloWorld::ccTouchesMoved(CCSet *touches, CCEvent *event){
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        //printf("%0.2f x %0.2f\n",touchLocation.x,touchLocation.y);
    }
}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event){
    for(auto it = touches->begin(); it != touches->end(); it++){
        CCTouch *touch = (CCTouch *)*it;
        CCPoint touchLocation = touch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        if(touch == leftTouch){
            leftTouch = NULL;
        }
        
        if(touch == rightTouch){
            rightTouch = NULL;
        }
    }
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
