#pragma once

class BlockData {
public:
	BlockData(BlockType type) {
		p_blockType = type;
	}
	~BlockData();
	bool isOn() { return p_isOn; }
	void setOn(bool state) { p_isOn = state; }
	BlockType getBlockType() { return p_blockType; }
private:
	bool p_isOn;
	BlockType p_blockType;
};

enum BlockType {
	Air = 0,
	Grass,
	Dirt,
	Stone,
	Sand,
	Water,
};
