package org.nbg.stac.empire.sns;

public class FBFriendPoj {

	private String userid;
	private String username;
	private String picture;
	private boolean isChecked;
	public boolean isChecked() {
		return isChecked;
	}
	public void setChecked(boolean isChecked) {
		this.isChecked = isChecked;
	}
	public String getUserid() {
		return userid;
	}
	public void setUserid(String userid) {
		this.userid = userid;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	
	public String getPicture() {
		return picture;
	}
	
	public void setPicture(String picture) {
		this.picture = picture;
	}
	
	public FBFriendPoj(){
		
	}
	public FBFriendPoj(String userid, String username) {
		this.userid = userid;
		this.username = username;
	}	
	
}