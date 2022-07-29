<script lang="ts">
    import { onMount } from 'svelte'
    import { getUser, deleteUser, showToast, changeUserPassword } from '../logic'
    import { navigate } from "svelte-navigator"
    import profileImage from "../../assets/profile.png";
    import { fade } from 'svelte/transition'

    let email : string
    let password : string
    let newPassword : string
    let disabled : boolean = true

    onMount(async () => {
        let response = await getUser()
        if(response != null){
            email = response.email
            disabled = false
        }
	})

    function validate() : boolean {
        if(password.trim().length == 0){
            showToast("Validation error", "Please enter your password")
            return false
        }
        return true
    }
    async function handleSubmit(event): Promise<void> {
        if(!validate())
            return;
        const {submitter: submitButton} = event;
        if(submitButton.id == "save") {
            if(newPassword.trim().length < 4){
                showToast("Password error", "Password too short!")
                return
            }
            await changeUserPassword(email, password, newPassword)
        } else if(submitButton.id == "delete"){
            if( await deleteUser(email, password) )
                navigate("/")
        }
    }
</script>

<div class="form" on:submit|preventDefault={handleSubmit}>
    <img class="mb-3 mt-4" src={profileImage} alt="profile" height="120" in:fade = {{duration: 200}} />
    <h1 class="h3 fw-normal">Your profile</h1>
    <form in:fade = {{duration: 200}}>
        <div class="mb-3">
            <label for="_" class="form-label">Email</label>
            <input bind:value={email} type="email" class="form-control" readonly>
        </div>
        <div class="mb-3">
            <label for="_" class="form-label">Password</label>
            <input bind:value={password} type="password" class="form-control" placeholder="Your current password" required disabled = {disabled}>
        </div>
        <div style="padding:15px;"></div>
        <div class="mb-3">
            <label for="_" class="form-label">Change Password</label>
            <input bind:value={newPassword} type="password" class="form-control" placeholder="New password" disabled = {disabled}>
        </div>
        <div style="padding:20px;"></div>
        <button id="save" class="w-100 mb-3 mt-3 btn btn-primary" type="submit" disabled = {disabled}>Save</button>
        <hr class="mb-3 dropdown-divider"/>
        <button id="delete" class="w-100 btn btn-danger" type="submit" disabled = {disabled} >Delete account</button>
    </form>
</div>

<style>
    .form {
        width: 100%;
        display: flex;
        align-items: center;
        flex-direction: column;
        justify-content: flex-start;
    }
    form {
        width: 100%;
        max-width: 350px;
        padding: 30px;
    }
</style>