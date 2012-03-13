#include "TwitterSimulator.h"
#include "TWitterManager.h"
#include "ofMain.h"

TwitterSimulator::TwitterSimulator(TwitterManager& manager) 
	:manager(manager)
	,gui(4000)
	,take_screenshot(false)
	,fx_mirror(false)
	,fx_flip(false)
	,fx_pixelate(false)
	,fx_shake(false)
	,fx_invert(false)
	,fx_swirl(false)
	,fx_posterize(false)
    ,fx_pixelate_x(5.0f)
    ,fx_pixelate_y(5.0f)
	,fx_shake_displace(0.01f)
	,fx_shake_speed(1.001f)
	,fx_shake_waves(5.0f)
	,fx_bounce_number(6.0f)
	,fx_bounce_amplitude(0.4f)
	,fx_bounce_duration(3.5f)
{
	
	gui.addString("fake_tweet", fake_tweet);
	gui.addCallback("simulate_tweet", 1, this);
	gui.addCallback("reload_commands", 2, this);
	gui.addCallback("take_screenshot", 3, this);
	gui.addCallback("reload_badwords", 4, this);
	gui.addCallback("save_settings", 5, this);
	gui.addCallback("bounce_fx", 6, this); 
	gui.addCallback("shock_fx", 7, this);
}

TwitterSimulator::~TwitterSimulator() {
}

void TwitterSimulator::saveSettings() {
	gui.save(settings_file);
}

void TwitterSimulator::loadSettings() {
	gui.load(settings_file);
}

void TwitterSimulator::setup(const string& settingsFile) {
	settings_file = settingsFile;
}

void TwitterSimulator::guiCallback(int v) {
	if(v == 1 && fake_tweet.length())  {
		printf("> fake tweet\n");
		rtt::Tweet tweet;
		tweet.setText(fake_tweet);
		manager.parseTweet(tweet);		
	}
	else if(v == 2) {
		printf("> reload commands\n");
		manager.getCommands().load();
	}
	else if(v == 3) {
		take_screenshot = true;
	}
	else if(v == 4) {
		printf("> reload bad words\n");
		manager.reloadBadWords();
	}
	else if(v == 5) {
		printf("> save settings.\n");
		saveSettings();
	}
	else if(v == 6) {
		printf("> bounce: duration: %f seconds, %f bounces, %f amplitude.\n", fx_bounce_duration, fx_bounce_number, fx_bounce_amplitude);
		fx->bounce(true, fx_bounce_duration, fx_bounce_number, fx_bounce_amplitude);
	}
	else if(v == 7) {
		printf("> shock!\n");
		fx->shockwave(true, 3.5);
	}
}

void TwitterSimulator::update() {
	gui.update();
	
	if(gui.didValueChange()) {
		fx->mirror(fx_mirror);
		fx->flip(fx_flip);
		fx->pixelate(fx_pixelate, fx_pixelate_x, fx_pixelate_y);
		fx->shake(fx_shake, fx_shake_speed, fx_shake_displace, fx_shake_waves);
		fx->invert(fx_invert);
		fx->swirl(fx_swirl, fx_swirl_radius, fx_swirl_angle);
		fx->posterize(fx_posterize);
	}
	
}

void TwitterSimulator::setEffects(Effects& f) {
	fx = &f;
	gui.addBool("fx_mirror", fx_mirror);
	gui.addBool("fx_flip", fx_flip);
	gui.addBool("fx_shake", fx_shake);
	gui.addBool("fx_pixelate", fx_pixelate);
	gui.addBool("fx_invert", fx_invert);
	gui.addBool("fx_swirl", fx_swirl);
	gui.addBool("fx_posterize", fx_posterize);
	gui.addFloat("fx_pixelate_x", fx_pixelate_x);
	gui.addFloat("fx_pixelate_y", fx_pixelate_y);
	gui.addFloat("fx_shake_displace", fx_shake_displace);
	gui.addFloat("fx_shake_speed", fx_shake_speed);
	gui.addFloat("fx_shake_waves", fx_shake_waves);
	gui.addFloat("fx_bounce_duration", fx_bounce_duration);
	gui.addFloat("fx_bounce_number", fx_bounce_number);
	gui.addFloat("fx_bounce_amplitude", fx_bounce_amplitude);
	gui.addFloat("fx_swirl_angle", fx_swirl_angle);
	gui.addFloat("fx_swirl_radius", fx_swirl_radius);
	
}