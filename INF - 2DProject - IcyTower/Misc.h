#pragma once

VideoMode loadResolution() {
	FILE* fp;
	VideoMode windowres = VideoMode(400, 400);
	fp = fopen("data\\resolution.dat", "r+b");
	if (fp == NULL)
		return windowres;
	fread(&windowres, sizeof(VideoMode), 1, fp);
	fclose(fp);
	return windowres;
}

void addPlaycount() {
	FILE* fp;
	int playcount = 0;
	double playtime = 0;
	fp = fopen("data\\Playtime.dat", "r+b");
	if (fp != NULL) {
		fread(&playcount, sizeof(int), 1, fp);
		fread(&playtime, sizeof(double), 1, fp);
		fclose(fp);
	}
	playcount++;
	fp = fopen("data\\Playtime.dat", "w+b");
	if (fp == NULL) {
		perror("Error with saving playcunt to Playtime.dat");
		return;
	}
	fwrite(&playcount, sizeof(int), 1, fp);
	fwrite(&playtime, sizeof(double), 1, fp);
	fclose(fp);
}

void addPlaytime(Clock& clock) {
	FILE* fp;
	int playcount = 0;
	double playtime = 0;
	fp = fopen("data\\Playtime.dat", "r+b");
	if (fp != NULL) {
		fread(&playcount, sizeof(int), 1, fp);
		fread(&playtime, sizeof(double), 1, fp);
		fclose(fp);
	}
	playtime += (double)clock.restart().asSeconds();
	fp = fopen("data\\Playtime.dat", "w+b");
	if (fp == NULL) {
		perror("Error with saving playtime to Playtime.dat");
		return;
	}
	fwrite(&playcount, sizeof(int), 1, fp);
	fwrite(&playtime, sizeof(double), 1, fp);
	fclose(fp);
}